"""Convert the reported EM2D registrations (file "Registration-Parameters")
   for each image into a set of transformations, each of which transforms
   the model coordinates so that they superpose on the corresponding class
   average (assuming that the class average image lies in the xy plane
   with its origin at (0,0,0)). Print each transformation in a form suitable
   for including in an IHM mmCIF file."""

from __future__ import print_function
import IMP.algebra
import IMP.atom
import math

def get_centroid(fname):
    """Return the coordinates of the centroid of the given PDB file"""
    m = IMP.Model()
    mh = IMP.atom.read_pdb(fname, m, IMP.atom.CAlphaPDBSelector())
    return IMP.core.get_centroid(IMP.atom.get_leaves(mh))

def get_registrations(fname):
    """Yield information from the given registration parameters file"""
    with open(fname) as fh:
        for line in fh:
            if line.startswith('#') or '|' not in line:
                continue
            spl = line.split('|')
            # Image #, quaternion plus x and y shift
            yield(int(spl[0]), [float(spl[i]) for i in range(5,9)],
                  [float(spl[i]) for i in range(9,11)])

def get_transformations(registrations, centroid, pixel_size, image_size):
    """Yield a set of transformations from the given registration file"""
    # See do_project_particles() in IMP's modules/em2d/src/project.cpp for
    # the underlying algorithm used here.

    # Reported rotation quaternion assumes both the model centroid and the
    # center of the class average are at the origin, so we need to translate
    # accordingly
    to_centroid = IMP.algebra.Transformation3D(IMP.algebra.Vector3D(-centroid))
    image_origin = pixel_size * image_size * 0.5

    # em2d code uses the unusual convention of x=row and y=column, so correct
    # for this with a 90 degree rotation about the origin
    xy_rot = IMP.algebra.get_rotation_about_axis(
                           IMP.algebra.Vector3D(0., 0., 1.), -0.5 * math.pi)
    cleanup = IMP.algebra.Transformation3D(xy_rot,
                          IMP.algebra.Vector3D(image_origin, image_origin, 0.))

    for num, quat, shift in get_registrations(registrations):
        rot = IMP.algebra.Rotation3D(quat)
        # Reported shift is in pixels, so convert to angstroms
        tnsl = IMP.algebra.Vector3D(pixel_size * shift[0],
                                    pixel_size * shift[1], 0.)
        trans = cleanup * IMP.algebra.Transformation3D(rot, tnsl) * to_centroid
        yield num, trans
