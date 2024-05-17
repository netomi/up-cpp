// SPDX-FileCopyrightText: 2024 Contributors to the Eclipse Foundation
//
// See the NOTICE file(s) distributed with this work for additional
// information regarding copyright ownership.
//
// This program and the accompanying materials are made available under the
// terms of the Apache License Version 2.0 which is available at
// https://www.apache.org/licenses/LICENSE-2.0
//
// SPDX-License-Identifier: Apache-2.0

#ifndef UP_CPP_COMMUNICATION_NOTIFICATIONSINK_H
#define UP_CPP_COMMUNICATION_NOTIFICATIONSINK_H

#include <up-cpp/transport/UTransport.h>
#include <up-cpp/utils/Expected.h>
#include <uprotocol/v1/umessage.pb.h>
#include <uprotocol/v1/uri.pb.h>
#include <uprotocol/v1/ustatus.pb.h>

#include <memory>
#include <optional>
#include <utility>

namespace uprotocol::communication {

/// @brief Interface for uEntities to receive notifications.
///
/// Like all L2 client APIs, the NotificationSink is a wrapper on top of the L1
/// UTransport API; in this instance, it provides for the notification
/// receiving half of the notification model.
struct NotificationSink {
	using ListenCallback = transport::UTransport::ListenCallback;

	using StatusOrSink =
	    utils::Expected<std::unique_ptr<NotificationSink>, v1::UStatus>;

	/// @brief Create a notification sink to receive notifications.
	///
	/// The sink will remain active so long as the NotificationSink is held.
	/// Resetting the unique_ptr for the NotificationSink will automatically
	/// unregister the callback.
	///
	/// @param sink URI of this uE. The authority and entity will be replaced
	///             automatically with those found in the transport's default.
	/// @param callback Called when a notification is received.
	/// @param source_filter (Optional) URI to compare against notification
	///                      sources. Only notifications that match will be
	///                      forwarded tot he callback.
	///
	/// @returns
	///    * unique_ptr to a NotificationSink if the callback was connected
	///      successfully.
	///    * UStatus containing an error state otherwise.
	static StatusOrSink create(const v1::UUri& sink, ListenCallback&& callback,
	                           std::optional<v1::UUri>&& source_filter);

	~NotificationSink() = default;

protected:
	/// @brief Constructs a notification listener connected to a given
	///        transport.
	///
	/// @param transport Transport to receive notifications on.
	/// @param listener Handle for a callback connected and listening for
	///                 notifications.
	///
	/// @throws std::invalid_argument if listener is not connected.
	NotificationSink(std::shared_ptr<transport::UTransport> transport,
	                 transport::UTransport::ListenHandle&& listener);

private:
	std::shared_ptr<transport::UTransport> transport_;

	transport::UTransport::ListenHandle listener_;
};

}  // namespace uprotocol::communication

#endif  // UP_CPP_COMMUNICATION_NOTIFICATIONSINK_H
