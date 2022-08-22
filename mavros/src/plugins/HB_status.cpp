/*
 * Copyright 2013,2016,2021 Vladimir Ermakov.
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */
/**
 * @brief HB_status plugin
 * @file HB_status.cpp
 * @author Vladimir Ermakov <vooon341@gmail.com>
 *
 * @example HB_status.cpp
 * @addtogroup plugin
 * @{
 */
#include "std_msgs/msg/u_int8.hpp"
#include "rcpputils/asserts.hpp"
#include "mavros/mavros_uas.hpp"
#include "mavros/plugin.hpp"
#include "mavros/plugin_filter.hpp"


namespace mavros
{
namespace std_plugins
{
using mavlink::minimal::MAV_TYPE;
/**
 * @brief HB_status plugin.
 * @plugin HB_status
 *
 * get all mavlink heartbeats and publish to ROS.
 */
class HB_statusPlugin : public plugin::Plugin
{
public:

  /**
   * Plugin constructor.
   * It must first call initialization of base class.
   *
   * When second argument is passed, it's used as a sub-node name.
   * `node` class member will be initialized with it.
   *
   * You can use `node->create_xyz()` functions to make any node interfaces.
   */
  explicit HB_statusPlugin(plugin::UASPtr uas_)
  : Plugin(uas_, "HB_status")
  {
    auto state_qos = rclcpp::QoS(10).transient_local();
    heartbeat_status_pub = node->create_publisher<std_msgs::msg::UInt8>("heartbeat_status", state_qos);

  }

  /**
   * This function returns message subscriptions.
   *
   * Each subscription made by PluginBase::make_handler() template.
   * Two variations:
   *  - With automatic decoding and framing error filtering (see handle_heartbeat)
   *  - Raw message with framig status (see handle_systemtext)
   */
  Subscriptions get_subscriptions() override
  {
    return {
      make_handler(&HB_statusPlugin::handle_heartbeat),
    };
  }

private:
  /**
   * This function will be called to handle HEARTBEAT from any source
   * when framing is ok.
   *
   * @param[in] msg     raw message frame
   * @param[in] hb      decoded message (require a type from mavlink c++11 library)
   * @param[in] filter  an instance of that filter will determine conditions to call that function
   */
  rclcpp::Publisher<std_msgs::msg::UInt8>::SharedPtr heartbeat_status_pub;

  void handle_heartbeat(
    const mavlink::mavlink_message_t * msg [[maybe_unused]],
    mavlink::minimal::msg::HEARTBEAT & hb,
    plugin::filter::AnyOk filter [[maybe_unused]])
  {
    auto HB_msg = std_msgs::msg::UInt8();

    HB_msg.data = hb.type;
    heartbeat_status_pub->publish(HB_msg);

    // RCLCPP_INFO_STREAM(get_logger(), "HB_status::handle_heartbeat: " << hb.to_yaml());
    // if (hb.type == mavros::utils::enum_value(MAV_TYPE::GCS)){
      // RCLCPP_INFO_STREAM(get_logger(), "-----GCS!");
      // HB_msg.data = "connected";
      // heartbeat_status_pub->publish(HB_msg);
    // }
  }
};

}       // namespace std_plugins
}       // namespace mavros

#include <mavros/mavros_plugin_register_macro.hpp>  // NOLINT
MAVROS_PLUGIN_REGISTER(mavros::std_plugins::HB_statusPlugin)
