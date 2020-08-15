;;;;;;;;;;;;;;;
;; Questions ;;
;;;;;;;;;;;;;;;

; Scheme

(define (cddr s)
  (cdr (cdr s)))

(define (cadr s)
  'YOUR-CODE-HERE
  (car (cdr s))
)

(define (caddr s)
  'YOUR-CODE-HERE
  (car (cdr (cdr s)))
)

(define (sign x)
  'YOUR-CODE-HERE
  (cond ((< x 0) -1) ((> x 0) 1) (else 0)) 
  
)

(define (square x) (* x x))

(define (pow b n)
  'YOUR-CODE-HERE
  (if (= n 0)
      1
      (if (even? n)
          (square (pow b (/ n 2)))
          (* (square (pow b (/ (- n 1) 2))) b))) 

)

(define (unique s)
  'YOUR-CODE-HERE
  (define (remove item lst)
          (filter (lambda (x) (not (equal? item x))) lst))
  (if (null? s)
      s
      (cons (car s) (remove (car s) (unique (cdr s)))))
  
)
