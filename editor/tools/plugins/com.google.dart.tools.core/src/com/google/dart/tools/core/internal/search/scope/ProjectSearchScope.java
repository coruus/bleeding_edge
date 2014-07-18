/*
 * Copyright (c) 2011, the Dart project authors.
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
package com.google.dart.tools.core.internal.search.scope;

import com.google.dart.tools.core.model.DartElement;
import com.google.dart.tools.core.model.DartProject;
import com.google.dart.tools.core.search.SearchScope;

/**
 * Instances of the class <code>ProjectSearchScope</code> implement a search scope that encompasses
 * everything in the workspace.
 * 
 * @coverage dart.tools.core.search
 */
public class ProjectSearchScope implements SearchScope {

  private final DartProject projectScope;

  /**
   * Create a search scope that encompasses everything in the types included in the given project.
   * 
   * @param project the project defining which types are included in the scope
   */
  public ProjectSearchScope(DartProject project) {
    this.projectScope = project;
  }

  @Override
  public boolean encloses(DartElement element) {
    DartProject elementProject = element.getAncestor(DartProject.class);
    return elementProject.equals(projectScope);
  }

}
