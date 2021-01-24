### A lisp-like toy programming language written in C.

I wrote this interpreter to teach myself language design concepts. It is not meant to solve any real-world programming task. This is mainly because there are no input/output concepts except of a `print` function. Also there is not garbage collection or optimization for recursion so you will quickly reach the stack size limit of the OS.

Having said that, the language has all the features you need to write simple and complex algorithms and assemble them into larger building blocks: arithmetic, boolean logic, comparision, branching, functions, iteration through recursion.

I created and tested the interpreter only on __Debian 10__ with __GCC 8.3.0__.

## Usage

Build

```
$ ./build
```

Run test suites

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

(The `range` function in this example is defined in `lib/list.dl`)

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

The integer is the only number type in the language. There are no floating point numbers.

```clojure
(print -42)
```

__Strings__

Allowed escape sequences are `\n`, `\t`, `\"` and `\\`

```clojure
(print "hello\n")
```

__None__

The none type can be created by the `none` function. There is no literal to create it.

```clojure
(print (none))
```

__Lists__

A list can by created by the `list` function. There is no literal to create a list.

```clojure
(print (list 1 2 3))

; empty list
(print (list)) 

; A list can have mixed element types.
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

Expressions at the last position will be evaluated before they get bound to the name.

```clojure
(define nums (range 0 5 (-> x x)))
(print nums) ; (0, 1, 2, 3, 4)
```

### Functions

A function can be created by the `->` function, followed by a list of parameter names (optional), followed by the `locals` function (optional) and finally a body expression (required).

```clojure
; Function without parameters and without local variables.
(define answer (-> 42))
(print (answer)) ; 42

; Function with one parameter but without local variables.
(define double (-> num (* num 2)))
(print (double 3)) ; 6

; Funcion with one parameter and two local variables
(define first-plus-last (-> nums
    (locals
        start (head nums)
        end (last nums))
    (+ start end)))

(print (first-plus-last (list 3 1 2 6))) ; 9
```

Functions are data types too and can be passed to other functions. Passing functions can be done by name or inline.

```clojure
(define double (-> num (* num 2)))

; Pass function by name.
(print (map double (list 2 3 4))) ; (4, 6, 8)

; Pass function inline.
(print (map 
    (-> num (* num 2))
    (list 2 3 4))) ; (4, 6, 8)
```

### Dynamic Scoping

The language has dynamic scoping. That means that a global identifier may not only be shadowed by the local variables of the current executing functions but also by the function that has called the current function.

```clojure
(define x 3)

(define f1 (-> (print x)))

(define f2 (-> x (f1))) ; f2 shadows x

(f1) ; 3

(f2 42) ; 42
```

I don't consider dynamic scoping a good concept and most modern programming languages have static scoping. But it is very easy to implement.

### Build-in functions

__Boolean logic__

There is no special data type for boolean values. In a boolean context (such as the `if` function) the values `0` and `none` are treated as false, all other values are treated as true.

The `not` function returns `1` for falsy values and `0` for truthy values.

```clojure

; 0 and none are the only falsy values and `not` will return 1 for them.

(not 0) ; 1
(not (none)) ; 1

; Everything else will produce 0.

(not (list 1 2 3)) ; 0
(not (list)) ; 0
(not (-> x x)) ; 0
```

The `and` function takes at least 1 argument and returns the first falsy value of the argument list or the last argument.

```clojure
(and 1 2 3) ; 3
(and 1 0 3) ; 0
(and 1 2 0) ; 0
(and (list 1 2) "foo" "bar") ; "bar"
(and (none) "foo" "bar") ; <None>
```

The `or` function takes at least 1 argument. It returns the first truthy value of the argument list ar the last argument.

```clojure
(or 1 2 3) ; 1
(or 0 3 0) ; 3
(or 0 0 0) ; 0
(or (list 1 2) "foo" "bar") ; (1, 2)
(or (none) "foo" "bar") ; "foo"
```

__Comparision__

The build-in comparison functions work on intergers, strings, functions and none. To compare lists you have to use the library function `equal?` (see below). The functions return `0` or `1`

```clojure

; Integer comparision works as you would expect.

(= 3 2) ; 0
(< 3 2) ; 0
(> 3 2) ; 1
(<= 3 2) ; 0
(>= 3 2) ; 1 

; String comparision uses the `strcmp` function of the C library.
; It performs a character by character ascii value comparision of the strings.

(= "foo" "bar") ; 0
(< "foo" "bar") ; 0
(> "foo" "bar") ; 1
(<= "foo" "bar") ; 0
(>= "foo" "bar") ; 1

; Comparision of functions work by compare the references (pointers) of the functions.

(define f1 (-> 23))
(define f2 (-> 23))

(= f1 f1) ; 1
(= f1 f2) ; 0
(<= f1 f1); 1
(< f1 f2); This does not make sense so return 0.

; You can pass mixed types to the comparision functions which will result in 0.

(< 1 "foo"); 0
```

__Math functions__

As said above there are no floating point numbers so the divide function may return truncated numbers.

```clojure
(+ 5 2)

(- 5 2)

(* 5 2)

(/ 5 2)

(% 5 2) ; modulo
```

__Type checking__

```clojure
(int? 5) ; 1

(str? "foo") ; 1

(none? (none)) ; 1

(list? (list)) ; 1

(function? (-> x x)) ; 1
```

__List functions__

All build-in list functions work on strings too.

The `head` function returns the first element in a list or `none` if the list is empty.

```clojure
(head (list 1 2 3)) ; 1
(head (list)) ; none
```

The `tail` function returns the list without the first element or an empty list if the list is empty.

```clojure
(tail (list 1 2 3)) ; (2, 3)
(tail (list)) ; ()
```

The `last` function returns the last element in a list or `none` if the list is empty.

```clojure
(last (list 1 2 3)) ; 3
(last (list)) ; none
```

The `init` function returns the list without the last element or an empty list if the list is empty.

```clojure
(init (list 1 2 3)) ; (1, 2)
(init (list)) ; ()
```

The `empty?` function returns `1` if the list is empty and `0` otherwise.

```clojure
(empty? (list 1 2 3)) ; 0
(empty? (list)) ; 1
```

The `cons` function inserts an element to the start of a list.

```clojure
(cons 1 (list 2 3)) ; (1, 2, 3)
(cons 1 (list)); (1)
```

The `append` function inserts an element to the end of a list

```clojure
(append (list 1 2) 3) ; (1, 2, 3)
(append (list) 1) ; (1)
```

For strings the `cons` and `append` function behave the same. Also the element to append or prepend does not need to be a single character.

```clojure
(cons "f" "oobar") ; foobar
(append "f" "oobar") ; foobar
(cons "foo" "bar"); foobar
(append "foo" "bar"); foobar
```

__Branching__

There is only one construct for branching: the `if` function. It takes 3 arguments. If the first argument evaluates to true then the second argument will be resolved and returned. Otherwise the third argument will be resolved and returned.

```clojure
(if 1 2 3) ; 2

(if (< 5 2) 7 8) ; 8

(define items (list 1 2 3))

(if (< (len items) 10)
    (append items 42)
    items) ; (1, 2, 3, 42)
```

__Iteration__

There is not special construct for iteration (looping). Iteration must be accomplished by recursion.

For example, here is a function that counts strings in a list.

```clojure
(define count-strings (-> items count
    (if (empty? items) count
        (count-strings (tail items)
            (if (str? (head items))
                (+ count 1)
                count)))))

(count-string (list 1 2 "foo" 3 "bar") 0) ; 2
```

~~Input~~/Output

As said above, there is no input/ouptut in this language except of the `print` function.

The `print` function takes an arbitrary length of arguments of any type and prints its string representations on the screen. The function returns the first argument or `none` if no arguments where passed.

```clojure
(print 3) ; prints "3" and returns 3
(print 3 4 5) ; prints "345" and returns 3
(print 3 " " 4 " " 5) ; prints "3 4 5" and returns 3
(print "hello") ; prints "hello" and returns "hello"
(print "hello" "\n") ; prints "hello" and a newline and returns "hello"
(print (list 1 2 3)) ; prints "(1, 2, 3)" and returns (1, 2, 3)
(print (-> x x)) ; prints "<Function>" and returns that function
(print) ; prints nothing and returns `none`
```

Because `print` returns its first argument you can wrap it around any expression to inspect the resulting value:

```clojure
(map
    (-> x (print (* x x) " "))
    (list 1 2 3)) ; prints "1 4 9" and returns (1 4 9)
```

### Library functions

The `equal?` function takes two arguments of any type and performs a comparision. If the arguments are not lists then the functions behaves like the `=` build-in function. If the arguments are list then the function checks for deep equality of the lists.

```clojure
(define items1 (list (list 1 2 3) (list (list 3 4 5))))
(define items2 (list (list 1 2 3) (list (list 3 4 5))))
(define items3 (list (list 1 2 3) (list (list 3 4))))

(equal? items1 items2) ; 1
(equal? items1 items3) ; 0
```

The `len` function takes a list or a string and returns the count of its elements or characters.

```clojure
(len (list)) ; 0
(len (list 1 2 3)) ; 3
(len "foobar") ; 6
```

The `get` functions takes a list or a string and an index and returns the element or character at this index or `none`.

```clojure
(get (list) 4) ; `none`
(get "foobar" 0) ; "f"
```

The `find` function takes a list or a string and an arbitrary value and returns the index of the first occurence of that value or `-1` if nothing was found.

```clojure
(find (list) 3) ; `none`
(find (list 1 2 3) 3) ; 2
(find "foobar" "o") ; 1
```

The `range` function creates a list of nubmers that can be transformed by the passed function.

```clojure
(range 0 5 (-> x x)) ; (0, 1, 2, 3, 4)
(range 0 5 (-> x 1)) ; (1, 1, 1, 1, 1)
(range 0 5 (-> x (* x x))) ; (0, 1, 4, 9, 16) 
```

The `map`, `filter` and `reduce` (foldl) functions work as known from many other programming languages. They can treat lists and strings.

```clojure
(map (-> x (* x x)) (list 1 2 3)) ; (1, 4, 9)

(filter (-> c (not (= c "m"))) "lorem ipsum") ; "lore ispu"

(reduce (-> acc x (+ acc x) (list 1 2 3 4))) ; 10 
```

