package org.aperiodic;

import java.util.Map;
import java.util.LinkedHashMap;

public class PDSLabel {
  private String version;
  public Map<String,PDSValue> metadata;
  public Map<String,PDSObject> objects;

  public PDSLabel(String version, Map<String,PDSValue> metadata, Map<String,PDSObject> objects) {
    this.version = version;
    this.metadata = metadata;
    this.objects = objects;
  }

  public String getVersion() {
    return version;
  }
}
