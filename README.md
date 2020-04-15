[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.1209565.svg)](https://doi.org/10.5281/zenodo.1209565)

These scripts demonstrate the use of [IMP](https://integrativemodeling.org/),
[MODELLER](https://salilab.org/modeller/),
[FoXS](https://salilab.org/foxs/), and
[AllosMod](https://salilab.org/allosmod/), and
[Minimal Ensemble Search](http://bl1231.als.lbl.gov/saxs_protocols/mes.php)
in the modeling of the Nup133 protein in the *S. cerevisiae* Nuclear Pore
Complex (NPC). First, MODELLER is used to generate an initial
comparative model of Nup133 guided by FoXS fits to SAXS data. Then, the model
was subjected to conformational sampling with AllosMod, and finally Minimal
Ensemble Search was used to identify four models that together reproduced both
the SAXS data and a set of electron microscopy class averages. The final model
was also validated against a set of chemical cross-links, that were not used in
the modeling.

A full description of the scripts can be found in
[Integrative structure-function mapping of the nucleoporin Nup133 suggests a conserved mechanism for membrane anchoring of the nuclear pore complex](http://mcponline.org/content/early/2014/08/19/mcp.M114.040915).

## Steps

### Comparative modeling

Python scripts to generate comparative models of parts of the Nup133 can
be found in the `MODELLER` subdirectory. Each subdirectory corresponds to
modeling consistent with a single construct (directory names match the protein
IDs in Supplementary Table S1 in the publication, also available as
`SAXS/Nup133_tableS1.xls`). A number of candidate models were generated
for each construct, ranked by FoXS score, and the single best model selected
for subsequent steps.

 - `all_sjkim_final.py` in `MODELLER/23917` generates models consistent with
   construct 490-1157
 - `all_sjkim_final_23902a.py` in `MODELLER/23902` generates models consistent
   with construct 2-515
 - `all_sjkim_final_23904.py` in `MODELLER/23904C` takes the best scoring model
   23917 and generates models consistent with construct 490-1157
 - `all_sjkim_final_23904.py` in `MODELLER/23904` takes the best scoring model
   23904C and 23902a and generates models consistent with construct 2-1157 (the
   complete protein)

### AllosMod

The complete ensemble was generated by submission to the
[AllosMod](https://salilab.org/allosmod/) web server.

### Minimal Ensemble Search

Results from running Minimal Ensemble Search can be found in the
`outputs_foxs_ensemble_new/MES_results` directory. `new_mes4.log` contains
the final selected ensemble of four models, also present in this directory.
The electron microscopy class averages used are also present in the
`outputs_foxs_ensemble_new/ISAC_p150_t346_m30` directory.

### Validation against cross-links

Intramolecular cross-link data is available in
`Crosslinks/DSS_EDC_crosslinks.txt`. Simple Python scripts to calculate
residue-residue distances for the final ensemble can be found in the same
directory.

## Information

_Author(s)_: Seung Joong Kim

_Date_: September 2nd, 2014

_License_: [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)
This work is licensed under the Creative Commons Attribution-ShareAlike 4.0
International License.

_Last known good IMP version_: [![build info](https://integrativemodeling.org/systems/11/badge.svg?branch=master)](https://integrativemodeling.org/systems/) [![build info](https://integrativemodeling.org/systems/11/badge.svg?branch=develop)](https://integrativemodeling.org/systems/)

_Testable_: Yes.

_Parallelizeable_: No

_Publications_:
 - Seung Joong Kim\*, Javier Fernandez-Martinez\*, Parthasarathy Sampathkumar\*, Anne Martel, Tsutomu Matsui, Hiro Tsuruta, Thomas Weiss, Yi Shi, Ane Markina-Inarrairaegui, Jeffery B. Bonanno, J. Michael Sauder, Stephen K. Burley, Brian T. Chait, Steven C. Almo, Michael P. Rout, and Andrej Sali, [Integrative structure-function mapping of the nucleoporin Nup133 suggests a conserved mechanism for membrane anchoring of the nuclear pore complex](http://mcponline.org/content/early/2014/08/19/mcp.M114.040915), Molecular & Cellular Proteomics, 2014, mcp.M114.040915.

 \*These authors contributed equally to this work as co-first authors.
