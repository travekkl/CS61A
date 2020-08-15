; Macros

(define-macro (list-of map-expr for var in lst if filter-expr)
  (define (filter fn vals)
    (if (null? vals)
        ()
        (if (fn (car vals))
            (cons (car vals) (filter fn (cdr vals)))
            (filter fn (cdr vals)))))
  (define (map fn vals)
     (if (null? vals)
         ()       
         (cons (fn (car vals))             
         (map fn (cdr vals)))))
  (list 'map (list 'lambda (list var) map-expr) (list 'filter (list 'lambda (list var) filter-expr) lst))
)