;; This file contains target machine instruction templates. We are forced to provide at least one condition/instruction
;; otherwise the build will fail (zero-sized array error)

;; We will extend this file later, for now we only provide two
;; instructions, a simple add (reg = reg + reg) that uses a dummy
;; condition and a nop (interestingly, gc refuses to build without it)
;; -----------------------------------------------------------------------

(define_insn "addsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (plus:SI (match_operand:SI 1 "register_operand" "r")
                 (match_operand:SI 2 "register_operand" "r")))]
  "minirisc_dummy_insn_cond(SImode)"
  "%0 = %1 + %2;"
)

;; -----------------------------------------------------------------------

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
)