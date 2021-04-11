# script to generate A* results
path = 'mclcs-opt/sum/'
results = {}
lines = []
with open(path + 'clcs-alg-0bs-0guidance-0.csv') as f:
    lines = f.readlines()
for line in lines:
    # index file runs obj_mean obj_sd ttot_mean
    if line.split()[0] != 'index':
        results[line.split()[0]] = (line.split()[0], '0', '0', '0', '-')

with open(path + 'clcs-alg-7bs-0guidance-0.csv') as f:
    lines = f.readlines()
for line in lines:
    if line.split()[0] != 'index':
        results[line.split()[0]] = (line.split()[0], line.split()[2], round(float(line.split()[3]), 1), line.split()[4], round(float(line.split()[5]), 1))

with open(path + 'r7.csv', 'w') as f:
    f.write('index file runs obj_mean obj_sd ttot_mean\n')
    for key, val in results.iteritems():
        f.write(key.ljust(16))
        f.write(val[0].ljust(16))
        f.write(str(val[1]).rjust(5))
        f.write(str(val[2]).rjust(15))
        f.write(str(val[3]).rjust(15))
        f.write(str(val[4]).rjust(10))
        f.write('\n')
