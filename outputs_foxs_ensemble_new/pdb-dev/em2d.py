import os
import csv
import ihm.location
import ihm.dataset
import ihm.restraint
from get_transformations import *

em2d_dir = '../ISAC_p150_t346_m30'

class EM2DFits(object):

    num_images = 23
    image_name_format = '23904_%02d.pgm'

    def __init__(self, assembly):
        self.assembly = assembly
        self.im_per_class, self.ccc = \
                 self.read_csv('../MES_results/Nup133_tableS3.csv')

    def read_csv(self, fname):
        """Parse Table S3 to get # of images and CCC per class"""
        im_per_class = [None] * self.num_images
        ccc = [None] * self.num_images
        with open(fname, 'rb') as fh:
            for row in csv.reader(fh):
                try:
                    class_id = int(row[0]) - 1
                    im_per_class[class_id], ccc[class_id] = \
                            self._parse_csv_fit(row)
                except ValueError:
                    continue
        return im_per_class, ccc

    def _parse_csv_fit(self, row):
        ccc = [None]*4
        z = [None]*4
        (class_id, im_per_class, cutoff, z[0], ccc[0], z[1], ccc[1],
         z[2], ccc[2], z[3], ccc[3]) = row
        im_per_class = int(im_per_class)
        z = [float(x) for x in z]
        ccc = [float(x) for x in ccc]
        return im_per_class, ccc

    def add_all(self):
        for image in range(self.num_images):
            fname = os.path.join(em2d_dir, self.image_name_format % image)
            l = ihm.location.InputFileLocation(fname)
            dataset = ihm.dataset.EM2DClassDataset(location=l)
            r = ihm.restraint.EM2DRestraint(dataset=dataset,
                     assembly=self.assembly, segment=False,
                     # From ../2_em2d_0.sh
                     pixel_size_width=2.28739, pixel_size_height=2.28739,
                     number_raw_micrographs=self.im_per_class[image],
                     number_of_projections=1000, image_resolution=15.)
            yield r

    def add_model(self, nmodel, model, restraints):
        """Add fit data to the given model for all restraints (images)"""
        assert(len(restraints) == self.num_images)

        centroid = get_centroid(model.file_name)
        pixel_size = 2.28739
        image_size = 128 # todo: check
        regparam = os.path.join(model.file_name[:-4], 'registration.params')
        for num, trans in get_transformations(regparam, centroid,
                                              pixel_size, image_size):
            ccc = self.ccc[num][nmodel]
            rot = trans.get_rotation()
            shift = [e for e in trans.get_translation()]
            rm = [[e for e in rot.get_rotation_matrix_row(i)] for i in range(3)]

            restraints[num].fits[model] = ihm.restraint.EM2DRestraintFit(
                                      rot_matrix=rm, tr_vector=shift,
                                      cross_correlation_coefficient=ccc)
