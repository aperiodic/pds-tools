package org.aperiodic;

import java.util.Map;
import java.util.LinkedHashMap;

public class PDSObject {
  private String name;
  public Map<String,PDSValue> attributes;

  public PDSObject(String name, Map<String,Object> attributes) {
    this.name = name;
    this.attributes = new LinkedHashMap<String,PDSValue>();
    for (Map.Entry<String,Object> attr : attributes.entrySet()) {
      this.attributes.put(attr.getKey(), new PDSValue(attr.getValue()));
    }
  }

  public String getName() {
    return name;
  }
}
