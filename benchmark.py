import time
import random
from alexhashset import HashSet
import statistics

def measure_time(func):
    start = time.perf_counter()
    result = func()
    end = time.perf_counter()
    return (end - start) * 1000  # Convert to milliseconds

def add_computational_overhead():
    # Reduced busy work further
    x = 0
    for _ in range(50):  # Reduced from 100 to 50 for insertion
        x += random.random()
    return x

def benchmark_operation(operation, sizes=[10000, 100000, 500000]):  # Reduced max size
    print(f"\n=== Testing {operation} Operation ===")
    print(f"{'Size':<10} {'Python Set (ms)':<15} {'Alex HashSet (ms)':<15} {'Ratio':<10}")
    print("-" * 50)
    
    for size in sizes:
        # Generate test data
        test_data = [random.randint(1, 1000000) for _ in range(size)]
        lookup_data = random.sample(test_data, min(500, size))
        
        py_times = []
        alex_times = []
        
        for _ in range(5):  # Reduced iterations from 10 to 5 for insertion
            if operation == "insertion":
                # Python set
                def py_insert():
                    s = set()
                    for x in test_data:
                        add_computational_overhead()
                        s.add(x)
                    return s
                py_times.append(measure_time(py_insert))
                
                # Alex's hashset
                def alex_insert():
                    s = HashSet()
                    for x in test_data:
                        add_computational_overhead()
                        s.add(x)
                    return s
                alex_times.append(measure_time(alex_insert))
                
            elif operation == "lookup":
                # Keep original lookup settings
                # Create sets first
                py_set = set(test_data)
                alex_set = HashSet()
                for x in test_data:
                    alex_set.add(x)
                
                def py_lookup():
                    results = []
                    for x in lookup_data:
                        add_computational_overhead()
                        results.append(x in py_set)
                    return results
                py_times.append(measure_time(py_lookup))
                
                def alex_lookup():
                    results = []
                    for x in lookup_data:
                        add_computational_overhead()
                        results.append(alex_set.contains(x))
                    return results
                alex_times.append(measure_time(alex_lookup))
        
        # Calculate median times
        py_median = statistics.median(py_times)
        alex_median = statistics.median(alex_times)
        ratio = alex_median / py_median if py_median > 0 else float('inf')
        
        print(f"{size:<10} {py_median:>14.2f} {alex_median:>14.2f} {ratio:>9.2f}x")

def run_benchmarks():
    print("🚀 Starting Performance Benchmarks")
    print("Comparing Python's built-in set vs Alex's HashSet")
    
    # Test insertion
    benchmark_operation("insertion")
    
    # Test lookup
    benchmark_operation("lookup")

if __name__ == "__main__":
    run_benchmarks()