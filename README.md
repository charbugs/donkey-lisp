An incomplete lisp-like toy programming language written in C.

Note: I wrote this interpreter to teach myself language design concepts. It is not meant to solve any real-world programming task.

I created and tested the interpreter only on __Debian 10__ with __GCC 8.3.0__.

## Usage

Build

```
$ ./build
```

Tests

```
$ ./runtests
```

Hello world

```
$ echo '(print "Hello, world!\n")' > hello.dl
$ ./bin/donkey hello.dl

Hello, world!
```

Include library functions

(The `range` function is defined in `lib/list.dl`)

```
$ echo '(print (range 0 10 (-> x x)) "\n")' > libtest.dl
$ ./bin/donkey lib/* libtest.dl

(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
```

## Examples

Fibonacci numbers

```clojure
(define fib (-> n
    (if (<= n 1) n
        (+ (fib (- n 1)) (fib (- n 2))))))

(print (fib 9)) ; 34
```

Repeat a string n times

```clojure
(define repeat (-> src n
    (_repeat src "" n)))

(define _repeat (-> src dst n
    (if (= 0 n) dst
        (_repeat src (append dst src) (- n 1)))))

(print (repeat "oh " 3) "!") ; oh oh oh !
```

Average of list of numbers

(The result may be truncated as there are no floating point numbers)

```clojure
(define average (-> nums
    (locals
        count (len nums))
    (if (= 0 count) 0
        (/ (reduce (-> acc i (+ acc i)) nums 0) count))))

(print (average (list 1 2 3 4 5))) ; 3
```

## Reference

### Data types

__Integers__

There are no floating point numbers.

```clojure
(print -42)
```

__Strings__

Allowed escape sequences are `\n`, `\t`, `\"` and `\\`

```clojure
(print "hello\n")
```

__None__

There is no literal for the none type. It can be produced by the `none` function.

```clojure
(print (none))
```

__Lists__

There is no literal for the list type. A list must be createt by the `list` function.

```clojure
(print (list 1 2 3))

; empty list
(print (list)) 

; a list can have mixed element types
(print (list 1 "foo" 3)) 

; list of lists
(print (list (list 1 2) (list 3 4))) 
```

### Define

The `define` function binds a value to a name and sets it to the global scope. 

```clojure
(define text "lorem ipsum")
(print text)
```

Expressions at the last position are resolved to a value before they get bound to a name.

```clojure
(define nums (range 0 5))
(print nums) ; (0, 1, 2, 3, 4)
```

### Functions

A function can be created by the `->` function, followed by a list of parameter names (optional), followed by the `locals` function (optional) and finally a body expression (required).

```clojure
; function without parameters and without local variables
(define answer (-> 42))
(print (answer)) ; 42

; function with one parameter but without local variables
(define double (-> num (* num 2)))
(print (double 3)) ; 6

; funcions with parameters and local variables
(define first-plus-last (-> nums
    (locals
        start (head nums)
        end (last nums))
    (+ start end)))

(print (first-plus-last (list 3 1 2 6))) ; 9
```

