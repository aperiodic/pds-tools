package org.aperiodic;

import java.util.Map;
import java.util.LinkedHashMap;

public class PDSObject {
  private String name;
  public Map<String,PDSValue> attributes;

  public PDSObject(String name, Map<String,PDSValue> attributes) {
    this.name = name;
    this.attributes = attributes;
  }

  public String getName() {
    return name;
  }
}
