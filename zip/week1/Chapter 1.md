CS61A第一章阅读笔记

1.1

0.我们的智力之旅一旦出发就不能回头了，我们也永远都不能对此抱有期待。

1.All computing begins with representing information, specifying logic to process it, and designing abstractions that manage the complexity of that logic. 

表示信息、配置逻辑、设计抽象以管理逻辑复杂性

2.These fundamental ideas have long been taught using the classic textbook ***Structure and Interpretation of Computer Programs*** ([SICP](http://mitpress.mit.edu/sicp)) by Harold Abelson and Gerald Jay Sussman with Julie Sussman. 

3.A language isn't something you learn so much as something you join.

—[Arika Okrent](http://arikaokrent.com/)

语言并不是你学到的东西，而是你参与的东西。

4.IDLE快捷键操作

Alt+P：上一条命令

Alt+N：下一条命令

5.And, as imagination bodies forth the forms of things to unknown, and the poet's pen

turns them to shapes, and gives to airy nothing a local habitation and a name.

—William Shakespeare, A Midsummer-Night's Dream

6.Learning to interpret errors and diagnose the cause of unexpected errors is
called *debugging*.



1.2

 1.Every powerful language has three such mechanisms:

- **primitive expressions and statements**, which represent the simplest building blocks that the language provides,
- **means of combination**, by which compound elements are built from simpler ones, and
- **means of abstraction**, by which compound elements can be named and manipulated as units.

2.The possibility of binding names to values and later retrieving those values by
name means that the interpreter must maintain some sort of memory that keeps
track of the names, values, and bindings. This memory is called an
***environment***.

3.With multiple assignment, *all* expressions to the right of `=` are evaluated before *any* names to the left are bound to those values. As a result of this rule, swapping the values bound to two names can be performed in a single statement.

```python
>>> x, y = 3, 4.5
>>> y, x = x, y
>>> x
4.5
>>> y
3
```

4.A nested expression of calls to `print` highlights the non-pure character of the function.

```python
>>> print(print(1), print(2))
1
2
None None
```



1.3——2020.05.25

http://composingprograms.com/pages/13-defining-new-functions.html

Defining New Functions

1.Now we will learn about *function definitions*, a much more powerful abstraction
technique by which a name can be bound to compound operation, which can then be
referred to as a unit.

2.**How to define a function.** Function definitions consist of a `def` statement that indicates a `<name>` and a comma-separated list of named `<formal parameters>`, then a `return` statement, called the function body, that specifies the `<return expression>` of the function, which is an expression to be evaluated whenever the function is applied:

```
def <name>(<formal parameters>):
    return <return expression>
```

3.Both `def` statements and assignment statements bind names to values, and any existing bindings are lost.  For example, `g` below first refers to a function of no arguments, then a number, and then a different function of two arguments.

```
>>> def g():
        return 1
>>> g()
1
>>> g = 2
>>> g
2
>>> def g(h, i):
        return h + i
>>> g(1, 2)
3
```

4.An environment in which an expression is evaluated consists of a sequence of
*frames*, depicted as boxes. Each frame contains *bindings*, each of which
associates a name with its corresponding value. There is a single *global*
frame. Assignment and import statements add entries to the first frame of the
current environment. So far, our environment consists only of the global frame.

5.The name of a function is repeated twice, once in the frame and again as part of the function itself.
The name appearing in the function is called the *intrinsic name*. The name in
a frame is a *bound name*. There is a difference between the two: different
names may refer to the same function, but that function itself has only one
intrinsic name.

1.4.

Designing Functions

http://composingprograms.com/pages/14-designing-functions.html

1.Fundamentally, the qualities of good functions all reinforce the idea that
functions are abstractions.

好的函数具有抽象的属性

2.A function definition will often include documentation describing the function, called a *docstring*, which must be indented along with the function body. Docstrings are conventionally triple quoted. The first line describes the job of the function in one line. The following lines can describe arguments and clarify the behavior of the function:

```
>>> def pressure(v, t, n):
        """Compute the pressure in pascals of an ideal gas.

        Applies the ideal gas law: http://en.wikipedia.org/wiki/Ideal_gas_law

        v -- volume of gas, in cubic meters
        t -- absolute temperature in degrees kelvin
        n -- particles of gas
        """
        k = 1.38e-23  # Boltzmann's constant
        return n * k * t / v
```

When you call `help` with the name of a function as an argument, you see its docstring (type `q` to quit Python help).

```
>>> help(pressure)
```

1.5

Control

http://composingprograms.com/pages/15-control.html

1.Expressions can also be executed as statements, in which case they are
evaluated, but their value is discarded.

2.**Conditional statements**. A conditional statement in Python consists of a series of headers and suites: a required `if` clause, an optional sequence of `elif` clauses, and finally an optional `else` clause:

```
if <expression>:
    <suite>
elif <expression>:
    <suite>
else:
    <suite>
```

3.Pythonincludes several false values, including 0, `None`, and the *boolean* value
`False`.  All other numbers are true values.

4.**Boolean operators**. Three basic logical operators are also built into Python:

```
>>> True and False
False
>>> True or False
True
>>> not False
True
```

与或非

5.To evaluate the expression `<left> and <right>`:

1. Evaluate the subexpression `<left>`.
2. If the result is a false value `v`, then the expression evaluates to `v`.
3. Otherwise, the expression evaluates to the value of the subexpression `<right>`.

To evaluate the expression `<left> or <right>`:

1. Evaluate the subexpression `<left>`.
2. If the result is a true value `v`, then the expression evaluates to `v`.
3. Otherwise, the expression evaluates to the value of the subexpression `<right>`.

To evaluate the expression `not <exp>`:

1. Evaluate `<exp>`; The value is `True` if the result is a false value, and `False` otherwise.

   

6.A `while` clause contains a header expression followed by a suite:

```
while <expression>:
    <suite>
```

To execute a `while` clause:

1. Evaluate the header's expression.

2. If it is a true value, execute the suite, then return to step 1.

   

7.A *test* is a mechanism for systematically performing this verification

8.**Assertions.**  Programmers use `assert` statements to verify expectations, such as the output of a function being tested. An `assert` statement has an expression in a boolean context, followed by a quoted line of text (single or double quotes are both fine, but be consistent) that will be displayed if the expression evaluates to a false value.

```
>>> assert fib(8) == 13, 'The 8th Fibonacci number should be 13'
```

9.**Doctests.** Python provides a convenient method for placing simple tests directly in the docstring of a function. The first line of a docstring should contain a one-line description of the function, followed by a blank line.  A detailed description of arguments and behavior may follow. In addition, the docstring may include a sample interactive session that calls the function:

```
>>> def sum_naturals(n):
        """Return the sum of the first n natural numbers.

        >>> sum_naturals(10)
        55
        >>> sum_naturals(100)
        5050
        """
        total, k = 0, 1
        while k <= n:
            total, k = total + k, k + 1
        return total
```

Then, the interaction can be verified via the [doctest module](http://docs.python.org/py3k/library/doctest.html). Below, the `globals` function returns a representation of the global environment, which the interpreter needs in order to evaluate expressions.

```
>>> from doctest import testmod
>>> testmod()
TestResults(failed=0, attempted=2)
```

To verify the doctest interactions for only a single function, we use a `doctest` function called `run_docstring_examples`.  This function is (unfortunately) a bit complicated to call.  Its first argument is the function to test.  The second should always be the result of the expression `globals()`, a built-in function that returns the global environment.  The third argument is `True` to indicate that we would like "verbose" output: a catalog of all tests run.

```
>>> from doctest import run_docstring_examples
>>> run_docstring_examples(sum_naturals, globals(), True)
Finding tests in NoName
Trying:
    sum_naturals(10)
Expecting:
    55
ok
Trying:
    sum_naturals(100)
Expecting:
    5050
ok
```

When the return value of a function does not match the expected result, the `run_docstring_examples` function will report this problem as a test failure.

When writing Python in files, all doctests in a file can be run by starting Python with the doctest command line option:

```
python3 -m doctest <python_source_file>
```



1.6

2020.05.27

Higher-Order Functions

http://composingprograms.com/pages/16-higher-order-functions.html

1.Functions that manipulate functions are called higher-order functions.
This section shows how higher-order functions can serve as powerful abstraction
mechanisms, vastly increasing the expressive power of our language.

2.**Lexical scope.** Locally defined functions also have access to the name
bindings in the scope in which they are defined. 

3.The most critical part of this evaluation procedure is the transfer of the
parent for `sqrt_update` to the frame created by calling `sqrt_update`.
This frame is also annotated with `[parent=f1]`.



1.7

2020.05.30

## Recursive Functions

http://composingprograms.com/pages/17-recursive-functions.html

1.A function is called *recursive* if the body of the function calls the function
itself, either directly or indirectly.  That is, the process of executing the
body of a recursive function may in turn require applying that function again.
Recursive functions do not use any special syntax in Python, but they do require
some effort to understand and create.

2.Treating arecursive call as a functional abstraction has been called a *recursive leap
of faith*



HW02:

##########################

# Just for fun Questions #
##########################

def zero(f):
    return lambda x: x

def successor(n):
    return lambda f: lambda x: f(n(f)(x))

def one(f):
    """Church numeral 1: same as successor(zero)"""
    return lambda x: f(x)

def two(f):
    """Church numeral 2: same as successor(successor(zero))"""
    return lambda x: f(f(x))

three = successor(two)

def church_to_int(n):
    """Convert the Church numeral n to a Python integer.

    >>> church_to_int(zero)
    0
    >>> church_to_int(one)
    1
    >>> church_to_int(two)
    2
    >>> church_to_int(three)
    3
    """
    return n(lambda x: x + 1)(0)

def add_church(m, n):
    """Return the Church numeral for m + n, for Church numerals m and n.

    >>> church_to_int(add_church(two, three))
    5
    """
    return lambda f: lambda x: m(f)(n(f)(x))

def mul_church(m, n):
    """Return the Church numeral for m * n, for Church numerals m and n.

    >>> four = successor(three)
    >>> church_to_int(mul_church(two, three))
    6
    >>> church_to_int(mul_church(three, four))
    12
    """
    return lambda f: m(n(f))

def pow_church(m, n):
    """Return the Church numeral m ** n, for Church numerals m and n.

```Python
>>> church_to_int(pow_church(two, three))
8
>>> church_to_int(pow_church(three, two))
9
"""
return n(m)
```