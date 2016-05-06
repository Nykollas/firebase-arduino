//
// Copyright 2016 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef FIREBASE_ARDUINO_H
#define FIREBASE_ARDUINO_H

#include "Firebase.h"
#include "FirebaseObject.h"

#ifndef FIREBASE_JSONBUFFER_SIZE
#define FIREBASE_JSONBUFFER_SIZE 200
#endif // FIREBASE_JSONBUFFER_SIZE

/**
 * Gateway class for Arduino clients to interact with the Firebase backend.
 * This implementation is designed to follow Arduino best practices and favor
 * simplicity over all else. For more complicated usecases and more control
 * see the Firebase class in Firebase.h.
 */
class FirebaseArduino {
 public:
  /**
   * Must be called first. This sets the class up for use.
   * \param host Your firebase db server domain name, usually X.firebaseIO.com.
   * \param auth Optional authentication for the db, a Secret or Token.
   */
  void begin(const char* host, const char* auth = "");

  /**
   * Adds the data in value to the list located at path. Equivilent to the
   * REST API's POST.
   * You should check success() after calling.
   * \param path The path inside of your db to the list you wish to update.
   * \param value Data that you wish to add to the list.
   * \return The child name where the data was written.
   */
  String push(const String& path, const JsonVariant& value);

  /**
   * Writes the data in value to the node located at path Equivilent to the
   * REST API's PUT.
   * You should check success() after calling.
   * \param path The path inside of your db to the node you wish to update.
   * \param value Data that you wish to write.
   */
  void set(const String& path, const JsonVariant& value);


  /**
   * Gets the value located at path.
   * You should check success() after calling.
   * \param path The path inside of your db of the node you wish to retrieve.
   * \return The data located at that path. This may either be a single element
   * or it may be a json structure. Will only be populated if success() is true.
   */
  FirebaseObject get(const char* path);

  /**
   * Remove the variable, and possibly entire tree, located at path.
   * You should check success() after calling.
   * \param path The path inside of your db to the node you wish to remove,
   * including all of its children.
   */
  void remove(const char* path);

  /**
   * Starts streaming any changes made to the node located at path, including
   * any of its children.
   * You should check success() after calling.
   * This changes the state of this object. Once this is called you may start
   * monitoring available() and calling readEvent() to get new events.
   * \param path The path inside of your db to the node you wish to monitor.
   */
  void stream(const char* path);

  /**
   * Checks if there are new events available. This is only meaningful once
   * stream() has been called.
   * \return If a new event is ready.
   */
  bool available();

  /**
   * Reads the next event in a stream. This is only meaningful once stream() has
   * been called.
   * \return Object will have ["type"] that describes the event type, ["path"]
   * that describes the effected path and ["data"] that was updated.
   */
  FirebaseObject readEvent();

  /**
   * \return Whether the last command was successful.
   */
  bool success();

  /**
   * \return Whether the last command failed.
   */
  bool failed();

  /**
   * \return Error message from last command if failed() is true.
   */
  const String& error();
 private:
  String host_;
  String auth_;
  FirebaseError error_;
  std::unique_ptr<FirebaseHttpClient> http_;
};

extern FirebaseArduino Firebase;

#endif // FIREBASE_ARDUINO_H
