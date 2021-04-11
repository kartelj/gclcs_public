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

class ResultSummary:
  def __init__(self):
    self.result_list = []
    self.avg_obj = 0.0
    self.avg_time = 0.0
    self.sum_obj = 0
    self.sum_opt = 0
    self.sum_time = 0
    self.percentage = 0.0

def print_results(results):
  for index, result in results.iteritems():
    print '{0}: {1} - {2}'.format(result.index, result.obj, result.time)

def extract_index(columns):
    col_file = columns[0]
    index = col_file.split('/')[-1][:-4] # remove .out
    return index

def read_textfile(fname):
  lines = []
  with open(path + fname) as f:
    lines = f.readlines()
  return lines

def aggregate(results):
  agg = {}
  for index, result in results.iteritems():
    agg_index = index[:-2]
    if not agg_index in agg:
      agg[agg_index] = ResultSummary()
    agg[agg_index].result_list.append(result)
  for index, summary in agg.iteritems():
    sum_obj = 0
    sum_opt = 0
    sum_time = 0
    for result in summary.result_list:
      sum_obj += result.obj
      sum_time += result.time
      sum_opt += result.opt
    summary.sum_obj = sum_obj
    summary.sum_time = sum_time
    summary.sum_opt = sum_opt
    summary.avg_obj = float(sum_obj) / len(summary.result_list)
    summary.avg_time = float(sum_time) / len(summary.result_list)
    summary.avg_opt = float(sum_opt) / len(summary.result_list)
    summary.percentage = 100.0 * float(sum_obj) / float(sum_opt)
  return agg

def print_plot_data(p, sigma, n):
  print 'Plot data for p={0}, sigma={1}, n={2}'.format(p, sigma, n)
  print 'BS:'
  print '{',
  for index, summary in summaries.iteritems():
    item1 = summary.result_list[0]
    if item1.p == p:
      if item1.sigma == sigma:
        if item1.n == n:
          print '({0}, {1})'.format(item1.m, summary.avg_obj),
  print '};'
  print 'A*:'
  print '{',
  for index, summary in summaries.iteritems():
    item1 = summary.result_list[0]
    if item1.p == p:
      if item1.sigma == sigma:
        if item1.n == n:
          print '({0}, {1})'.format(item1.m, summary.avg_opt),
  print '};'
  print ''

def print_plot_data_sol_quality(results, sigma, write_to, alg):
  #print 'Plot for solution qualities for sigma=' + str(sigma)
  write_to.write('\\addplot+[] coordinates\n')
  grouped = {}
  for index, result in results.iteritems():
    if not result.p in grouped:
      grouped[result.p] = []
    if (result.sigma == sigma):
      grouped[result.p].append(result)
  
  write_to.write('{')
  for p in sorted(grouped.keys(), reverse = True):
    result_list = grouped[p]  
    group_sum = 0.0
    group_count = 0
    
    for result in result_list:
      group_sum += (100.0 * float(result.obj) / float(result.opt))
      group_count += 1
    write_to.write('({0}, {1:.3f})'.format(1.0 / p, (group_sum / float(group_count))))
  write_to.write('};\n')
  write_to.write('\\addlegendentry{' + alg +'}\n\n')

def print_plot_data_sol_quantity(results, sigma, write_to, alg):
  #print 'Plot for solution quantities for sigma=' + str(sigma)
  write_to.write('\\addplot+[] coordinates\n')
  grouped = {}
  for index, result in results.iteritems():
    if not result.p in grouped:
      grouped[result.p] = []
    if (result.sigma == sigma):
      grouped[result.p].append(result)
  
  write_to.write('{')
  for p in sorted(grouped.keys(), reverse = True):
    result_list = grouped[p]  
    group_opt = 0
    group_count = 0
    
    for result in result_list:
      group_count += 1
      if (result.obj == result.opt):
        group_opt += 1
    write_to.write('({0}, {1:.3f})'.format(1.0 / p, (100.0 * group_opt / float(group_count))))
  write_to.write('};\n')
  write_to.write('\\addlegendentry{' + alg +'}\n\n')

def read_results_for_alg(alg, fname):
  print 'ALG: ' + alg + ''
  print '-------'
  results = {}
  lines = read_textfile('sum-clcs-alg-7bs-0guidance-0.txt')
  for line in lines:
    if line.split()[0] != 'index' and line.split()[0] != 'file':
      columns = line.split() # FORMAT: file obj ttot ittot tbest
      index = extract_index(columns)
      results[index] = Result(index, columns[1], columns[2])
      results[index].opt = results[index].obj
  
  lines = read_textfile(fname)
  for line in lines:
    if line.split()[0] != 'index' and line.split()[0] != 'file':
      columns = line.split()
      index = extract_index(columns)
      if index in results:
        results[index].obj = int(columns[1])
        results[index].time = float(columns[2])
  
  summaries = aggregate(results)

  print 'writing qualities to files for sigma 4 and 20...'
  print_plot_data_sol_quality(results=results, sigma=4, write_to=f_quality_4, alg=alg)
  print_plot_data_sol_quality(results=results, sigma=20, write_to=f_quality_20, alg=alg)

  print 'writing quantities to files for sigma 4 and 20...'
  print_plot_data_sol_quantity(results=results, sigma=4, write_to=f_quantity_4, alg=alg)
  print_plot_data_sol_quantity(results=results, sigma=20, write_to=f_quantity_20, alg=alg)

  
  print 'Results where the optimum was not reached:'
  for index, result in results.iteritems():
    if result.obj != result.opt:
      print 'Instance {0}: {1}/{2}'.format(index, result.obj, result.opt)
  
  print '----------------------------------------'

  # for index, summary in summaries.iteritems():
  #   print '{0}: {1:.1f} - {2:.1f}'.format(index, summary.avg_obj, summary.percentage)

################################# main code #################################
path = 'mclcs-opt/sum/'
inp_algos = [ 'BS-Prob', 'BS-UB' , 'BS-EX' , 'Greedy', 'Appr.' ]
inp_files = [ 'sum-clcs-alg-3bs-2000guidance-1-k_filter-100.txt',
              'sum-clcs-alg-3bs-2000guidance-0-k_filter-100.txt',
              'sum-clcs-alg-3bs-2000guidance-3-k_filter-100.txt',
              'sum-clcs-alg-1bs-0guidance-0.txt',
              'sum-clcs-alg-0bs-0guidance-0.txt']

f_quantity_4 = open("plot_quantity_4.txt", "w")
f_quantity_20 = open("plot_quantity_20.txt", "w")
f_quality_4 = open("plot_quality_4.txt", "w")
f_quality_20 = open("plot_quality_20.txt", "w")


for alg, fname in itertools.izip(inp_algos, inp_files):
  read_results_for_alg(alg, fname)

f_quantity_4.close()
f_quantity_20.close()
f_quality_4.close()
f_quality_20.close()

# print ''
# for n in [100, 500, 1000]:
#   print_plot_data(p=20, sigma=20, n=n)
# print ''

