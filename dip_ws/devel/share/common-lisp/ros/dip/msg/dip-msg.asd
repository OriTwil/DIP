
(cl:in-package :asdf)

(defsystem "dip-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "pid_driver" :depends-on ("_package_pid_driver"))
    (:file "_package_pid_driver" :depends-on ("_package"))
  ))