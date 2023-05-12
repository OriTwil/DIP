;; Auto-generated. Do not edit!


(when (boundp 'dip::pid_driver)
  (if (not (find-package "DIP"))
    (make-package "DIP"))
  (shadow 'pid_driver (find-package "DIP")))
(unless (find-package "DIP::PID_DRIVER")
  (make-package "DIP::PID_DRIVER"))

(in-package "ROS")
;;//! \htmlinclude pid_driver.msg.html


(defclass dip::pid_driver
  :super ros::object
  :slots (_vel _omega ))

(defmethod dip::pid_driver
  (:init
   (&key
    ((:vel __vel) 0.0)
    ((:omega __omega) 0.0)
    )
   (send-super :init)
   (setq _vel (float __vel))
   (setq _omega (float __omega))
   self)
  (:vel
   (&optional __vel)
   (if __vel (setq _vel __vel)) _vel)
  (:omega
   (&optional __omega)
   (if __omega (setq _omega __omega)) _omega)
  (:serialization-length
   ()
   (+
    ;; float64 _vel
    8
    ;; float64 _omega
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _vel
       (sys::poke _vel (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _omega
       (sys::poke _omega (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _vel
     (setq _vel (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _omega
     (setq _omega (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get dip::pid_driver :md5sum-) "7ce61d077baf56134fc51389b267fce5")
(setf (get dip::pid_driver :datatype-) "dip/pid_driver")
(setf (get dip::pid_driver :definition-)
      "float64 vel
float64 omega
")



(provide :dip/pid_driver "7ce61d077baf56134fc51389b267fce5")


