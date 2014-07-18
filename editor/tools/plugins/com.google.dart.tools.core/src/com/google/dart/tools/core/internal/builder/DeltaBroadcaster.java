/*
 * Copyright (c) 2014, the Dart project authors.
 * 
 * Licensed under the Eclipse Public License v1.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 * 
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */
package com.google.dart.tools.core.internal.builder;

/**
 * Instances of {@code DeltaBroadcaster} notify one or more {@link DeltaListener}s of
 * {@link ResourceDeltaEvent}s.
 */
public class DeltaBroadcaster {

  protected DeltaListener listener;

  /**
   * Add a listener interested in receiving change information
   * 
   * @param listener the listener (not {@code null})
   */
  public void addDeltaListener(DeltaListener listener) {
    this.listener = DeltaListenerList.add(this.listener, listener);
  }

}
