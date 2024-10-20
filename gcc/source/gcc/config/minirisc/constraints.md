;;-------------------------------------------------------------------------------------------------------------------------
;; Constraints
;;
;; Basic constraints, define a range and pick a letter (see gcc internals).
;; When implementing instructions, specify constraints by using this letter.
;;
;; For constraints defined for our minirisc target we use capital letters.
;; For every letter in use (e.g. J) a function will be generated (e.g. satisfies constraint J) that subsequently can be used to  
;; build target predicates.
;;
;; NOTE, be careful here, faulty constraints are often a source of obscure and hard to find errors.
;;-------------------------------------------------------------------------------------------------------------------------

(define_register_constraint "D" "GP_REGS"
    "@internal
    General-purpose machine registers.")

(define_constraint "I"
    "32-bit signed integer constraint."
    (and (match_code "const_int")
         (match_test "IN_RANGE (ival, -2147483648, 2147483647)")))

(define_constraint "K"
    "32-bit unsigned constraint."
    (and (match_code "const_int")
         (match_test "IN_RANGE (ival, 0, 4294967295)")))

(define_constraint "J"
    "12-bit signed integer constraint."
    (and (match_code "const_int")
         (match_test "IN_RANGE (ival, -2048, 2047)")))