#!/usr/bin/env python

import unittest
import os
import sys
import subprocess
import glob

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(sys.argv[0]), '..'))

class Tests(unittest.TestCase):

    def test_mmcif(self):
        """Test generation of mmCIF output"""
        os.chdir(os.path.join(TOPDIR, 'outputs_foxs_ensemble_new', 'pdb-dev'))
        if os.path.exists("nup133.cif"):
            os.unlink("nup133.cif")
        p = subprocess.check_call(["./make-mmcif.py"])
        # Check size of output file
        with open("nup133.cif") as fh:
            wcl = len(fh.readlines())
        self.assertEqual(wcl, 50803)

if __name__ == '__main__':
    unittest.main()
