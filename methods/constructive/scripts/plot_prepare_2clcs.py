class ResultSummary:
  def __init__(self, index, avg_obj, avg_time):
    self.index = index
    if len(index.split('_')) == 4:
      self.m = int(index.split('_')[0])
      self.n = int(index.split('_')[1])
      self.sigma = int(index.split('_')[2])
      self.p = int(index.split('_')[3])
    self.avg_obj = float(avg_obj)
    self.avg_time = float(avg_time)

def print_results(results):
  for index, result in results.iteritems():
    print '{0}: {1} - {2}'.format(result.index, result.avg_obj, result.avg_time)

def read_textfile(fname):
  lines = []
  with open(fname) as f:
    lines = f.readlines()
  return lines

def read_csvfile(fname):
  results = {}
  lines = read_textfile(fname)
  for line in lines:
    if line.split()[0] != 'index' and line.split()[0] != 'file':
      columns = line.split() # FORMAT: index file runs obj_mean obj_sd ttot_mean
      index = columns[0]
      results[index] = ResultSummary(index, columns[3], columns[5])
  return results

################################# main code #################################
results_chin  = read_csvfile('2clcs-opt/sum/clcs-alg-4bs-0guidance-0.csv')
results_deo   = read_csvfile('2clcs-opt/sum/clcs-alg-9bs-0guidance-0.csv')
results_ae    = read_csvfile('2clcs-opt/sum/clcs-alg-5bs-0guidance-0.csv')
results_astar = read_csvfile('2clcs-opt/sum/clcs-alg-7bs-0guidance-0.csv')
results_hung  = read_csvfile('2clcs-opt/sum/clcs-alg-8bs-0guidance-0.csv')
results_ir    = read_csvfile('2clcs-opt/sum/clcs-alg-6bs-0guidance-0.csv')
results = { 
  'Chin': results_chin,
  'Deo': results_deo,
  'AE': results_ae,
  'A$^*$': results_astar,
  'Hung': results_hung,
  'IR': results_ir,
 }

## plots with p as x-axis:
sigma = 20
for n in [100, 500, 1000]:
  with open('plot_xp_2clcs_n' + str(n), 'w') as out:
    for algorithm, result_list in results.iteritems():
      out.write('\\addplot+[smooth] coordinates\n')
      out.write('{')
      for index, item in sorted(result_list.iteritems(), key=lambda pair: -pair[1].p):
        if item.sigma == sigma and item.n == n:
          out.write('({0}, {1}) '.format(1.0 / item.p, item.avg_time))
      out.write('};\n')
      out.write('\\addlegendentry{' + algorithm +'}\n\n')

## plots with n as x-axis:
p = 20
for sigma in [4, 20]:
  with open('plot_xn_2clcs_sigma' + str(sigma), 'w') as out:
    for algorithm, result_list in results.iteritems():
      out.write('\\addplot+[smooth] coordinates\n')
      out.write('{')
      for index, item in sorted(result_list.iteritems(), key=lambda pair: pair[1].n):
        if item.sigma == sigma and item.p == p:
          out.write('({0}, {1}) '.format(item.n, item.avg_time))
      out.write('};\n')
      out.write('\\addlegendentry{' + algorithm +'}\n\n')

## plots for node stats:
nodes_astar = read_csvfile('2clcs-opt/sum/nodestats-clcs-alg-7bs.csv')
nodes_hung = read_csvfile('2clcs-opt/sum/nodestats-clcs-alg-8bs.csv')
nodes_deo = read_csvfile('2clcs-opt/sum/nodestats-clcs-alg-9bs.csv')
nodes_created = {
  'A$^*$': nodes_astar,
  'Deo': nodes_deo,
  'Hung': nodes_hung
}
for sigma in [4, 20]:
  for n in [100, 500, 1000]:
    with open('plot_nodescreated_2clcs_n' + str(n) + "_sigma" + str(sigma), 'w') as out:
      max_possible = ''
      for algorithm, result_list in nodes_created.iteritems():
        out.write('\\addplot+[smooth] coordinates\n')
        out.write('{')
        for index, item in sorted(result_list.iteritems(), key=lambda pair: -pair[1].p):
          if item.sigma == sigma and item.n == n:
            out.write('(1/{0}, {1}) '.format(item.p, item.avg_obj))
            if algorithm == 'A$^*$':
              max_possible += '({0}, {1}) '.format(1.0 / item.p, item.n * item.n * item.n / item.p)
        out.write('};\n')
        out.write('\\addlegendentry{' + algorithm +'}\n\n')

      out.write('\\addplot+[smooth] coordinates\n')
      out.write('{')
      out.write(max_possible)
      out.write('};\n')
      out.write('\\addlegendentry{Max}\n\n')