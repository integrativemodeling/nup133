import os
import ihm.location
import ihm.dataset

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
