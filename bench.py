import subprocess
import psutil
import os

CFLAGS = '-std=c99 -O3'

iterations = 10

benchmarks = [
    'stemt_hm_bench',
    'sheredom_hashmap_h_bench',
    'tidwall_hashmap_c_bench',
]



def memory_usage_psutil(pid):
    # return the memory usage in bytes
    process = psutil.Process(pid)
    mem = process.memory_info()[0]
    return mem


def compile_bench_mark(name):
    cmd = f'cc {CFLAGS} -o {name} src/{name}.c -I.'
    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    proc.wait()
    if proc.wait() != 0:
        print(f'compilation of "{name}" failed')
        exit(1)

def clean_bench_mark(name):
    os.remove(f'./{name}')

def run_bench_mark(name):
    output = ''
    mem_usage = 0

    cmd = f'./{name}'
    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    for line in proc.stdout:
        output += line.decode()
        if 'PASSED' in line.decode():
            mem_usage = memory_usage_psutil(proc.pid)
            proc.communicate(input=b'stop\n')
            if proc.wait() != 0:
                print(f'"{name}" returned non zero')
                exit(1)
            break
    return output,mem_usage

print('compiling benchmarks:')
for name in benchmarks:
    print(f'  {name}')
    compile_bench_mark(name)

print(f'testing {iterations} iterations')
print('running benchmarks...')
results = {}
mem_usages = {}
for i in range(iterations):
    print(f'  iteration: {i+1}/{iterations}')
    for name in benchmarks:
        print(f'    {name}')
        output,mem_usage = run_bench_mark(name)
        for line in output.split('\n'):
            if '_Bench' in line and 'ns' in line:
                first,second= line.split('_Bench.')
                lib_name = first.split(' ')[-1]
                if lib_name not in results.keys():
                    results[lib_name] = {}
                function, time_str = second.split(' ')
                time = float(time_str[1:-4])
                if function not in results[lib_name].keys():
                    results[lib_name][function] = []
                results[lib_name][function].append(time)
                if lib_name not in mem_usages.keys():
                    mem_usages[lib_name] = []
                mem_usages[lib_name].append(mem_usage)
print("\ndone")

print("results:")
for function in results['stemt_HM'].keys():
    print(f'--- {function} ---')
    for lib_name in results.keys():
        if function not in results[lib_name].keys():
            print(f'Error: function "{function}" not implemented for lib "{lib_name}"')
            exit(1)
        avg = sum(results[lib_name][function])/len(results[lib_name][function])
        factor = avg/(sum(results['stemt_HM'][function])/len(results['stemt_HM'][function]))
        if avg > 1000000:
            avg_ms = avg / 1000000.0
            print(f'{lib_name:20}: {avg_ms:>10.2f} ms : {factor:>10.2f}x')
        else:
            print(f'{lib_name:20}: {avg:>10.2f} ns : {factor:>10.2f}x')
    print()
print('--- memory usage at end ---')
for lib_name in mem_usages.keys():
    stats = mem_usages[lib_name]
    avg = (sum(stats)/len(stats))/1000000
    print(f'{lib_name:20}: {avg:>10.2f} MB')

for name in benchmarks:
    clean_bench_mark(name)


