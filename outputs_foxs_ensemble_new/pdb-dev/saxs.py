import os
import csv
import sys
import glob
import re
import ihm.location
import ihm.dataset

saxs_dir = '../../SAXS'

if sys.version_info[0] >= 3:
    def open_csv(fname):
        return open(fname, newline='')
else:
    def open_csv(fname):
        return open(fname, 'rb')

class SAXSFits(object):
    """Parse the SAXS csv file and add suitable fit data to the mmCIF file"""
    seqrange_re = re.compile('(\d+)\s*\-\s*(\d+)')

    def __init__(self, asym):
        self.asym = asym

    def add_from_csv(self):
        with open_csv(os.path.join(saxs_dir, 'Table6_SAXS.csv')) as fh:
            for row in csv.DictReader(fh):
                if row['FoXS fit score'] and row['Protein'] == 'NUP133':
                    yield self._add_one(row)

    def _add_one(self, row):
        protid = row['Protein ID']
        profile = (glob.glob('%s/%s_*.sub' % (saxs_dir, protid))
                   + glob.glob('%s/%s_*.dat' % (saxs_dir, protid)))[0]
        l = ihm.location.InputFileLocation(profile,
                             details = row['Notes'] if row['Notes']
                                                    else None)
        dataset = ihm.dataset.SASDataset(location=l)
        m = self.seqrange_re.match(row['Sequence coverage'])
        seqrange = (int(m.group(1)), int(m.group(2)))
        assembly = ihm.Assembly([self.asym(*seqrange)], name='SAXS assembly')
        r = ihm.restraint.SASRestraint(dataset=dataset, assembly=assembly,
                segment=False, fitting_method='FoXS')
        return r
        # todo: do something with fit data:
        # row['Rg'], row['FoXS fit score']
