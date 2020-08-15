
; Tail recursion

(define (replicate x n)
  (define (help x n acc)
          (if (= n 0)
              acc
              (help x (- n 1) (cons x acc))))
  (help x n nil)
  )

(define (accumulate combiner start n term)
  (define (help n acc)
          (if (= n 0)
              acc
              (help (- n 1) (combiner (term n) acc))))
  (help n start)
)

(define (accumulate-tail combiner start n term)
  (define (help n acc)
          (if (= n 0)
              acc
              (help (- n 1) (combiner (term n) acc))))
  (help n start)
)

; Streams

(define (map-stream f s)
    (if (null? s)
        nil
        (cons-stream (f (car s)) (map-stream f (cdr-stream s)))))

(define multiples-of-three
  (cons-stream 3 (map-stream (lambda (x) (+ x 3)) multiples-of-three)) 
)


(define (nondecreastream s)
  (cond
    ((null? s) nil)
    ((null? (cdr-stream s)) (cons-stream (list (car s)) nil))
    ((> (car s) (car (cdr-stream s))) (cons-stream (list (car s)) (nondecreastream (cdr-stream s))))
    (else (cons-stream (cons (car s) (car (nondecreastream (cdr-stream s)))) (cdr-stream (nondecreastream (cdr-stream s)))))
  )
)

(define finite-test-stream
    (cons-stream 1
        (cons-stream 2
            (cons-stream 3
                (cons-stream 1
                    (cons-stream 2
                        (cons-stream 2
                            (cons-stream 1 nil))))))))

(define infinite-test-stream
    (cons-stream 1
        (cons-stream 2
            (cons-stream 2
                infinite-test-stream))))
