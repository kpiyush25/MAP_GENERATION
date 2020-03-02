<joint name="camera_joint" type="fixed">
    <axis xyz="1 0 0" />
    <origin xyz="0 0 0.07" rpy="0 0 0" />
    <parent link="base_link"/>
    <child link="camera_link"/>
  </joint>

  <!-- Camera -->
  <link name="camera_link">
    <collision>
      <origin xyz="0 0 0" rpy="0 0 0"/>
      <geometry>
	<box size="${camera_link_box} ${camera_link_box} ${camera_link_box}"/>
      </geometry>
    </collision>

    <visual>
      <geometry>
	<box size="${camera_link_box} ${camera_link_box} ${camera_link_box}"/>
      </geometry>
    </visual>

    <inertial>
      <mass value="1e-6" />
      <origin xyz="0 0 0" rpy="0 0 0"/>
      <inertia ixx="1e-7" ixy="0" ixz="0" iyy="1e-7" iyz="0" izz="1e-7" />
    </inertial>
  </link>

  <!-- generate an optical frame http://www.ros.org/reps/rep-0103.html#suffix-frames
      so that ros and opencv can operate on the camera frame correctly -->

  <joint name="camera_optical_joint" type="fixed">
  <!--   these values have to be these values otherwise the gazebo camera image
        won't be aligned properly with the frame it is supposedly originating from -->
    <origin xyz="0 0 0" rpy="${-pi/2} 0 ${-pi/2}"/>
    <parent link="camera_link"/>
    <child link="camera_link_optical"/>
  </joint>

  <link name="camera_link_optical">
  </link>