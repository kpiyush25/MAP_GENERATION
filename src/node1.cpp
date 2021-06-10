#include <Offboard/node1.hpp>

mavros_msgs::State current_state_;
ros::Publisher local_pos_pub_;
geometry_msgs::PoseStamped pose_;
ros::ServiceClient set_mode_client_;
mavros_msgs::SetMode offb_set_mode_;
mavros_msgs::CommandBool arm_cmd_;
ros::Subscriber state_sub_;
ros::ServiceClient arming_client_;
void MapGenerator::init(ros::NodeHandle& nh) {
    state_sub_ = nh.subscribe<mavros_msgs::State>("mavros/state", 10, &MapGenerator::state_cb, this);
    local_pos_pub_ = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
    arming_client_ = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    set_mode_client_ = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");

    pose_.pose.position.x = 0;
    pose_.pose.position.y = 0;
    pose_.pose.position.z = 2;

    offb_set_mode_.request.custom_mode = "OFFBOARD";
    arm_cmd_.request.value = true;
}

void MapGenerator::state_cb(const mavros_msgs::State::ConstPtr& msg) {
    current_state_ = *msg;
}
int main(int argc, char** argv) {
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;
    MapGenerator mg;
    mg.init(nh);

    // the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    while (ros::ok() && !current_state_.connected) {
        ros::spinOnce();
        rate.sleep();
    }

    // send a few setpoints before starting
    for (int i = 100; ros::ok() && i > 0; --i) {
        local_pos_pub_.publish(pose_);
        ros::spinOnce();
        rate.sleep();
    }

    ros::Time last_request = ros::Time::now();

    while (ros::ok()) {
        if (current_state_.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0))) {
            if (set_mode_client_.call(offb_set_mode_) && offb_set_mode_.response.mode_sent) {
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } else {
            if (!current_state_.armed && (ros::Time::now() - last_request > ros::Duration(5.0))) {
                if (arming_client_.call(arm_cmd_) && arm_cmd_.response.success) {
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }

        local_pos_pub_.publish(pose_);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}