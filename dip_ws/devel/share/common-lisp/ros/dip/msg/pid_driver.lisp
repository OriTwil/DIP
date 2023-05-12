; Auto-generated. Do not edit!


(cl:in-package dip-msg)


;//! \htmlinclude pid_driver.msg.html

(cl:defclass <pid_driver> (roslisp-msg-protocol:ros-message)
  ((vel
    :reader vel
    :initarg :vel
    :type cl:float
    :initform 0.0)
   (omega
    :reader omega
    :initarg :omega
    :type cl:float
    :initform 0.0))
)

(cl:defclass pid_driver (<pid_driver>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <pid_driver>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'pid_driver)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name dip-msg:<pid_driver> is deprecated: use dip-msg:pid_driver instead.")))

(cl:ensure-generic-function 'vel-val :lambda-list '(m))
(cl:defmethod vel-val ((m <pid_driver>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dip-msg:vel-val is deprecated.  Use dip-msg:vel instead.")
  (vel m))

(cl:ensure-generic-function 'omega-val :lambda-list '(m))
(cl:defmethod omega-val ((m <pid_driver>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dip-msg:omega-val is deprecated.  Use dip-msg:omega instead.")
  (omega m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <pid_driver>) ostream)
  "Serializes a message object of type '<pid_driver>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'vel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'omega))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <pid_driver>) istream)
  "Deserializes a message object of type '<pid_driver>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'vel) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'omega) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<pid_driver>)))
  "Returns string type for a message object of type '<pid_driver>"
  "dip/pid_driver")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'pid_driver)))
  "Returns string type for a message object of type 'pid_driver"
  "dip/pid_driver")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<pid_driver>)))
  "Returns md5sum for a message object of type '<pid_driver>"
  "7ce61d077baf56134fc51389b267fce5")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'pid_driver)))
  "Returns md5sum for a message object of type 'pid_driver"
  "7ce61d077baf56134fc51389b267fce5")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<pid_driver>)))
  "Returns full string definition for message of type '<pid_driver>"
  (cl:format cl:nil "float64 vel~%float64 omega~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'pid_driver)))
  "Returns full string definition for message of type 'pid_driver"
  (cl:format cl:nil "float64 vel~%float64 omega~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <pid_driver>))
  (cl:+ 0
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <pid_driver>))
  "Converts a ROS message object to a list"
  (cl:list 'pid_driver
    (cl:cons ':vel (vel msg))
    (cl:cons ':omega (omega msg))
))
