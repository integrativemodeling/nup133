import ihm
try:
    import ihm.reference
except ImportError:
    pass
import os

# If we're running from an SGE job, override the from_accession() function
# to return cached values, since we don't have network access (needed
# to query PubMed or UniProt directly)

# Auto-generated by util/get_accession_map.py
accession_map = {
    'P36161': {'db_code':'NU133_YEAST', 'sequence':
        'MSEKKVHLRLRKELSVPIAVVENESLAQLSYEEESQASLMDISMEQQQLRLHSHFDNSKVFTENNR'
        'YIVKTLQTDYSSGFSNDDELNGYIDMQIGYGLVNDHKKVYIWNIHSTQKDTPYITVPFRSDDNDEI'
        'AVAPRCILTFPATMDESPLALNPNDQDETGGLIIIKGSKAIYYEDINSINNLNFKLSEKFSHELEL'
        'PINSSGGEKCDLMLNCEPAGIVLSTNMGRIFFITIRNSMGKPQLKLGKLLNKPFKLGIWSKIFNTN'
        'SSVVSLRNGPILGKGTRLVYITTNKGIFQTWQLSATNSHPTKLIDVNIYEAILESLQDLYPFAHGT'
        'LKIWDSHPLQDESSQLFLSSIYDSSCNETYYILSTIIFDSSSNSFTIFSTYRLNTFMESITDTKFK'
        'PKIFIPQMENANDTNEVTSILVMFPNAVVITQVNSKLDSSYSMRRKWEDIVSLRNDIDIIGSGYDS'
        'KSLYVLTKQMGVLQFFVKENEETNSKPEVGFVKSHVDQAVYFSKINANPIDFNLPPEISLDQESIE'
        'HDLKLTSEEIFHSNGKYIPPMLNTLGQHLSVRKEFFQNFLTFVAKNFNYKISPELKLDLIEKFEIL'
        'NCCIKFNSIIRQSDVLNDIWEKTLSNYNLTQNEHLTTKTVVINSPDVFPVIFKQFLNHVVFVLFPS'
        'QNQNFKLNVTNLINLCFYDGILEEGEKTIRYELLELDPMEVDTSKLPWFINFDYLNCINQCFFDFT'
        'FACEEEGSLDSYKEGLLKIVKILYYQFNQFKIWINTQPVKSVNANDNFININNLYDDNHLDWNHVL'
        'CKVNLKEQCIQIAEFYKDLSGLVQTLQTLDQNDSTTVSLYETFFNEFPKEFSFTLFEYLIKHKKLN'
        'DLIFRFPQQHDVLIQFFQESAPKYGHVAWIQQILDGSYADAMNTLKNITVDDSKKGESLSECELHL'
        'NVAKLSSLLVEKDNLDINTLRKIQYNLDTIDAEKNISNKLKKGEVQICKRFKNGSIREVFNILVEE'
        'LKSTTVVNLSDLVELYSMLDDEESLFIPLRLLSVDGNLLNFEVKKFLNALVWRRIVLLNASNEGDK'
        'LLQHIVKRVFDEELPKNNDFPLPSVDLLCDKSLLTPEYISETYGRFPIDQNAIREEIYEEISQVET'
        'LNSDNSLEIKLHSTIGSVAKEKNYTINYETNTVEY'},
}

if 'JOB_ID' in os.environ:
    if hasattr(ihm, 'reference'):
        def from_acc(cls, accession):
            return cls(accession=accession, **accession_map[accession])
        ihm.reference.UniProtSequence.from_accession = classmethod(from_acc)
