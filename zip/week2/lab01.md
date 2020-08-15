lab01

## What Would Python Display? (Part 1)

### Q1: WWPD: Control

> Use Ok to test your knowledge with the following "What Would Python Display?" questions:
>
> ```
> python3 ok -q control -u
> ```

```
>>> def xk(c, d):
...     if c == 4:
...         return 6
...     elif d >= 4:
...         return 6 + 7 + c
...     else:
...         return 25
>>> xk(10, 10)

23
>>> xk(10, 6)

23
>>> xk(4, 6)

17
>>> xk(0, 0)

25
>>> def how_big(x):
...     if x > 10:
...         print('huge')
...     elif x > 5:
...         return 'big'
...     elif x > 0:
...         print('small')
...     else:
...         print("nothin'")
>>> how_big(7)

big
>>> how_big(12)

huge
>>> how_big(1)

small
>>> how_big(-1)

nothing
>>> n = 3
>>> while n >= 0:
...     n -= 1
...     print(n)

2
1
0
```

> *Hint*: Make sure your `while` loop conditions eventually evaluate to a false value, or they'll never stop! Typing `Ctrl-C` will stop infinite loops in the interpreter.

```
>>> positive = 28
>>> while positive:
...    print("positive?")
...    positive -= 3

______
>>> positive = -9
>>> negative = -12
>>> while negative:
...    if positive:
...        print(negative)
...    positive += 3
...    negative += 3

______
```

### Q2: WWPD: Veritasiness

> Use Ok to test your knowledge with the following "What Would Python Display?" questions:
>
> ```
> python3 ok -q short-circuit -u
> ```

```
>>> True and 13

______
>>> False or 0

______
>>> not 10

______
>>> not None

______
>>> True and 1 / 0 and False

______
>>> True or 1 / 0 or False

______
>>> True and 0

______
>>> False or 1

______
>>> 1 and 3 and 6 and 10 and 15

______
>>> 0 or False or 2 or 1 / 0

______
>>> not 0

______
>>> (1 + 1) and 1

______
>>> 1/0 or True

______
>>> (True or False) and False
```