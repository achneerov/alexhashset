import time
import random
from alexhashset import HashSet
import statistics

def measure_time(func):
    start = time.perf_counter()
    result = func()
    end = time.perf_counter()
    return (end - start) * 1000  # Convert to milliseconds

def benchmark_operation(operation, sizes=[1000, 10000, 100000]):
    print(f"\n=== Testing {operation} Operation ===")
    print(f"{'Size':<10} {'Python Set (ms)':<15} {'Alex HashSet (ms)':<15} {'Ratio':<10}")
    print("-" * 50)
    
    for size in sizes:
        # Generate test data
        test_data = [random.randint(1, 1000000) for _ in range(size)]
        lookup_data = test_data[:100]  # Take first 100 elements for lookup tests
        
        # Test Python's set
        py_times = []
        alex_times = []
        
        for _ in range(5):  # Run each test 5 times
            if operation == "insertion":
                # Python set
                py_times.append(measure_time(lambda: set(test_data)))
                
                # Alex's hashset
                def alex_insert():
                    s = HashSet()
                    for x in test_data:
                        s.add(x)
                alex_times.append(measure_time(alex_insert))
                
            elif operation == "lookup":
                # Create sets first
                py_set = set(test_data)
                alex_set = HashSet()
                for x in test_data:
                    alex_set.add(x)
                
                # Python set lookup
                py_times.append(measure_time(
                    lambda: [x in py_set for x in lookup_data]
                ))
                
                # Alex's hashset lookup
                alex_times.append(measure_time(
                    lambda: [alex_set.contains(x) for x in lookup_data]
                ))
        
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