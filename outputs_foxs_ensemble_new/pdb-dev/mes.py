import os

mes_dir = '../MES_results'

def get_models_with_fractions():
    """Parse the MES log file to get the names of the output models and their
       population fractions"""
    def skip_to_models(log):
        for line in log:
            if line.startswith('    filename weight binary_score bit_count'):
                return
        raise ValueError("Bad MES log file")
    model_num = -1
    with open(os.path.join(mes_dir, 'new_mes4.log')) as log:
        skip_to_models(log)
        for model_line in log:
            if model_line == '\n':
                return
            s = model_line.strip().split(' ')
            model_num += 1
            yield (model_num, os.path.join(mes_dir, s[0][:-4] + '.pdb'),
                   float(s[1]))
