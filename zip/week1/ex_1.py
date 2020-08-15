from operator import mul

x = 5
def square(x):
    return mul(mul(x, 1), x)

def debug(x):
    x = x + 1
    return square(x)
