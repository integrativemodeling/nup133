import os
import ihm.location
import ihm.dataset
import ihm.restraint
from get_transformations import *

em2d_dir = '../ISAC_p150_t346_m30'

class EM2DFits(object):

    num_images = 23
    image_name_format = '23904_%02d.spi'

    def __init__(self, assembly):
        self.assembly = assembly

    def add_all(self):
        for image in range(self.num_images):
            fname = os.path.join(em2d_dir, self.image_name_format % image)
            l = ihm.location.InputFileLocation(fname)
            dataset = ihm.dataset.EM2DClassDataset(location=l)
            r = ihm.restraint.EM2DRestraint(dataset=dataset,
                     assembly=self.assembly, segment=False,
                     # From ../2_em2d_0.sh
                     pixel_size_width=2.28739, pixel_size_height=2.28739,
                     number_of_projections=1000, image_resolution=15.)
            yield r
            # todo: extract transformation from registration parameters file
            # todo: extract ccc from Table S3

    def add_model(self, model, restraints):
        """Add fit data to the given model for all restraints (images)"""
        assert(len(restraints) == self.num_images)

        centroid = get_centroid(model.file_name)
        pixel_size = 2.28739
        image_size = 128 # todo: check
        regparam = os.path.join(model.file_name[:-4], 'registration.params')
        for num, trans in get_transformations(regparam, centroid,
                                              pixel_size, image_size):
            rot = trans.get_rotation()
            shift = [e for e in trans.get_translation()]
            rm = [[e for e in rot.get_rotation_matrix_row(i)] for i in range(3)]

            restraints[num].fits[model] = ihm.restraint.EM2DRestraintFit(
                                                           rot_matrix=rm,
                                                           tr_vector=shift)
