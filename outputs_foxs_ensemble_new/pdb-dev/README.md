The files in this directory generate an mmCIF file of the complete Nup133
system for deposition [at PDB-Dev](https://pdb-dev.wwpdb.org/). These use
the [python-ihm library](https://github.com/ihmwg/python-ihm).

 - `make-mmcif.py` is the main script.
 - `em2d.py` adds information on the fit to EM class average data.
   - `get_transformations.py` parses the EM2D registration parameters file
     to get model-to-image transformations suitable for mmCIF.
 - `saxs.py` adds information on the fit to SAXS data.
 - `xlink.py` adds information on the fit to crosslink data.
 - `compmodel.py` reads in the comparative model used as a starting guess
   for the integrative modeling.
 - `mes.py` adds information on the Minimal Ensemble Search.
 - `pdb.py` adds coordinates of the final output models.
