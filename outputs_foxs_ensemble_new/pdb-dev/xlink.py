import os
import csv
import ihm.location
import ihm.dataset
import ihm.restraint
try:
    import ihm.cross_linkers
    cross_linkers = {'DSS':ihm.cross_linkers.dss, 'EDC':ihm.cross_linkers.edc}
except ImportError:
    cross_linkers = None

xlink_dir = '../../Crosslinks/'

class CrossLinkFits(object):

    def __init__(self, asym):
        self.asym = asym

    def add_restraints(self):
        """Parse the crosslinks text file and return a set of restraints"""
        fname = os.path.join(xlink_dir, 'DSS_EDC_crosslinks.txt')
        l = ihm.location.InputFileLocation(fname)
        d = ihm.dataset.CXMSDataset(l)
        for typ in ('DSS', 'EDC'):
            with open(fname) as fh:
                yield self.get_intra_restraint(d, typ, fh, self.asym)

    def get_intra_restraint(self, dataset, linker_type, fh, asym):
        # Distance thresholds, from Table II
        threshold = {'DSS':35., 'EDC':25.}
        distance = ihm.restraint.UpperBoundDistanceRestraint(
                                        threshold[linker_type])
        if cross_linkers:
            r = ihm.restraint.CrossLinkRestraint(dataset,
                                            linker=cross_linkers[linker_type])
        else:
            r = ihm.restraint.CrossLinkRestraint(dataset, linker_type)
        for line in fh:
            if line.startswith(linker_type):
                break
        for line in fh:
            if line == '\n': continue
            resids = line.split(',')
            if len(resids) == 2:
                # Numbering of xlink file is off by one compared to output model
                res = [asym.entity.residue(int(x) + 1) for x in resids]
                xxl = ihm.restraint.ExperimentalCrossLink(res[0], res[1])
                r.experimental_cross_links.append([xxl])
                xl = ihm.restraint.ResidueCrossLink(xxl, asym, asym, distance) 
                r.cross_links.append(xl)
            else:
                return r
