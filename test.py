from alexhashset import HashSet

# Create your hashset
s = HashSet()

# Test basic operations
s.add(42)
s.add(100)
s.add(42)  # Adding duplicate

# This should print:
# True (42 is in the set)
# True (100 is in the set)
# False (200 is not in the set)
print(s.contains(42))
print(s.contains(100))
print(s.contains(200))

# Test removal
s.remove(42)
print(s.contains(42))  # Should print False