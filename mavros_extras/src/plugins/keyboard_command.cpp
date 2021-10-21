//
// Created by uav on 2021/10/20.
// an attempt of sending a custom message from MAVROS to PX4
// ref:https://docs.px4.io/master/en/ros/mavros_custom_messages.html
//

#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>
#include <iostream>
#include <std_msgs/Char.h>

namespace mavros {
    namespace extra_plugins{

        class KeyboardCommandPlugin : public plugin::PluginBase {
        public:
            KeyboardCommandPlugin() : PluginBase(),
                                      nh("~keyboard_command")

            { };

            void initialize(UAS &uas_)
            {
                PluginBase::initialize(uas_);
                keyboard_sub = nh.subscribe("keyboard_sub", 10, &KeyboardCommandPlugin::keyboard_cb, this);
            };

            Subscriptions get_subscriptions()
            {
                return {/* RX disabled */ };
            }

        private:
            ros::NodeHandle nh;
            ros::Subscriber keyboard_sub;

            void keyboard_cb(const std_msgs::Char::ConstPtr &req)
            {
                std::cout << "Got Char : " << req->data <<  std::endl;

                //UAS_FCU(m_uas)->send_message_ignore_drop(req->data);  //  original version , cause error "invalid conversion"
                //UAS_FCU(m_uas)->send_message_ignore_drop((const mavlink_message_t*)(req->data)); //

                auto msg = mavlink::common::msg::KEY_COMMAND{};
                msg.command = req->data;
                UAS_FCU(m_uas)->send_message_ignore_drop(msg);
            }
        };
    }   // namespace extra_plugins
}   // namespace mavros

PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::KeyboardCommandPlugin, mavros::plugin::PluginBase)

