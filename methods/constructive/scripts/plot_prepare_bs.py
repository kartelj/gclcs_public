import itertools

class Result:
  def __init__(self, index, obj, time):
    self.index = index
    if len(index.split('_')) == 5:
      self.m = int(index.split('_')[1])
      self.n = int(index.split('_')[2])
      self.sigma = int(index.split('_')[3])
      self.p = int(index.split('_')[4].split('.')[0])
      self.instance = int(index.split('_')[4].split('.')[1])
    self.obj = int(obj)
    self.time = float(time)

def read_textfile(fname):
  lines = []
  with open(fname) as f:
    lines = f.readlines()
  return lines

def extract_index(columns):
    col_file = columns[0]
    index = col_file.split('/')[-1][:-4] # remove .out
    return index

def read_resultfile(fname):
  print 'Reading results from file {0}'.format(fname)
  results = {}
  lines = read_textfile(fname)
  for line in lines:
    if line.split()[0] != 'index' and line.split()[0] != 'file':
      columns = line.split() # FORMAT: file obj ttot ittot tbest
      index = extract_index(columns)
      results[index] = Result(index, columns[1], columns[2])
  return results

def read_templatefile(fname):
  template = ''
  with open('template/' + fname, 'r') as template_file:
    template = template_file.read()
  return template.split("<<PLOT_DATA>>");

################################# main code #################################
inp_algos = ['BS-Prob', 'BS-UB' , 'BS-EX' , 'BS-Min']
inp_algos_num = ['1', '0', '3', '5'] #['1', '0', '3', '5']
beamwidths = [1, 10, 100, 1000, 2000, 5000]
filter_sizes = [0, 10, 50, 100, 200]

print ''
print '-----------------------------'
print 'Plots for beamwidths'
print '-----------------------------'
print ''

## read input files for beamwidth plots
template_bw_obj = read_templatefile('bw_obj.tex')
template_bw_time = read_templatefile('bw_time.tex')

results_bw = []
for i in range(4):
  results_bw.append([])
  for bw in beamwidths:
    inp_fname = 'p-beamwidth/sum/sum-clcs-alg-3bs-{0}guidance-{1}-k_filter-100.txt'.format(bw, inp_algos_num[i])
    results_bw[i].append(read_resultfile(inp_fname));

## plots for different beamwidths
for sigma in ['All']: #[4,20]
  #for n in [100, 500, 1000]:
  for n in ['all']:
    print 'Generating plots for Sigma {0} and n {1} ...'.format(sigma, n)
    with open('p-beamwidth/plot_bw_n{1}.tex'.format(sigma, n), 'w') as out1, \
         open('p-beamwidth/plot_bw_time_n{1}.tex'.format(sigma, n), 'w') as out2:
      out1.write(template_bw_obj[0])
      out2.write(template_bw_time[0])
      for alg, result_list in itertools.izip(inp_algos, results_bw):
        out1.write('\\addplot coordinates\n')
        out2.write('\\addplot coordinates\n')
        out1.write('{')
        out2.write('{')
        for bw, results in itertools.izip(beamwidths, result_list):
          obj_sum = 0
          time_sum = 0.0
          item_count = 0
          for index, result in results.iteritems():
            #if result.n == n: # and result.sigma == sigma
            if True:
              obj_sum += result.obj
              time_sum += result.time
              item_count += 1
          print 'writing {0} result for bw={1} from {2} items'.format(alg, bw, item_count)
          out1.write('({0}, {1}) '.format(bw, float(obj_sum) / item_count))
          out2.write('({0}, {1}) '.format(bw, float(time_sum) / item_count))
        out1.write('};\n')
        out2.write('};\n')
      out1.write(template_bw_obj[1])
      out2.write(template_bw_time[1])

print ''
print '-----------------------------'
print 'Plots for filter sizes'
print '-----------------------------'
print ''

## read input files for k_filter plots
template_filter_obj = read_templatefile('filter_obj.tex')
template_filter_time = read_templatefile('filter_time.tex')

results_filter = []
for i in range(4):
  results_filter.append([])
  for k in filter_sizes:
    inp_fname = ''
    if k > 0:
      inp_fname = 'p-filter/sum/sum-clcs-alg-3bs-2000guidance-{0}-k_filter-{1}.txt'.format(inp_algos_num[i], k)
    else:
      inp_fname = 'p-filter/sum/sum-clcs-alg-3bs-2000guidance-{0}.txt'.format(inp_algos_num[i])
    results_filter[i].append(read_resultfile(inp_fname));

## plots for different filter sizes
for sigma in ['All']: #[4,20]:
  #for n in [100, 500, 1000]:
  for n in ['all']:
    print 'Generating plots for Sigma {0} and n {1} ...'.format(sigma, n)
    with open('p-filter/plot_filter_n{1}.tex'.format(sigma, n), 'w') as out1, \
         open('p-filter/plot_filter_time_n{1}.tex'.format(sigma, n), 'w') as out2:
      out1.write(template_filter_obj[0])
      out2.write(template_filter_time[0])
      for alg, result_list in itertools.izip(inp_algos, results_filter):
        out1.write('\\addplot coordinates\n')
        out2.write('\\addplot coordinates\n')
        out1.write('{')
        out2.write('{')
        for k, results in itertools.izip(filter_sizes, result_list):
          obj_sum = 0
          time_sum = 0.0
          item_count = 0
          for index, result in results.iteritems():
            #if result.n == n: # and result.sigma == sigma 
            if True:
              obj_sum += result.obj
              time_sum += result.time
              item_count += 1
          print 'writing {0} result for k={1} from {2} items'.format(alg, k, item_count)
          out1.write('({0}, {1}) '.format(k, float(obj_sum) / item_count))
          out2.write('({0}, {1}) '.format(k, float(time_sum) / item_count))
        out1.write('};\n')
        out2.write('};\n')
      out1.write(template_filter_obj[1])
      out2.write(template_filter_time[1])