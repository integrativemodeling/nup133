#!/usr/bin/env python

"""Convert Spider files to more common TIFF and PGM formats"""

import IMP.em2d
import os

srw = IMP.em2d.SpiderImageReaderWriter()
trw = IMP.em2d.TIFFImageReaderWriter()
for num in range(23):
    spi = '23904_%02d.spi' % num
    tiff = '23904_%02d.tiff' % num
    pgm = '23904_%02d.pgm' % num

    image = IMP.em2d.Image(spi, srw)
    image.write(tiff, trw)
    os.system('tifftopnm %s > %s' % (tiff, pgm))
