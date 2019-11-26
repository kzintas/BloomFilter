import itertools

from string import ascii_lowercase

#alphabet=[]
#keywords = ''.join((i) for i in product("ACTG", repeat = 4))
#print(keywords)
#keywords=product("ACTG", repeat = 3)
#print(keywords, file=open("Keys_exhaustive.txt", "w"))


#The yield statement suspends function’s execution and sends a value back
#  to caller, but retains enough state to enable function to resume where
#  it is left off. When resumed, the function continues execution 
# immediately after the last yield run. This allows its code to produce 
# a series of values over time, rather them computing them at once and 
# sending them back like a list
def foo(l):
    yield from itertools.product(l, repeat = 12) #itertools.product(*([l] * 3)) 

#It is good practice to use the with keyword when dealing with file 
# objects. The advantage is that the file is properly closed after its 
# suite finishes, even if an exception is raised at some point

with open("corpus.txt", "w") as myfile:
    for x in foo('ATCG'):
        myfile.write(''.join(x))
        myfile.write('\n')
