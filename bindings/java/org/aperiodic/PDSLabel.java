package org.aperiodic;

import java.util.Map;
import java.util.LinkedHashMap;

public class PDSLabel {
  private String version;
  public Map<String,PDSValue> metadata;
  public Map<String,PDSObject> objects;

  public PDSLabel( String version
                 , Map<String,Object> metadata
                 , Map<String,Map<String,Object>> objects
                 ) {
    this.version = version;
    this.metadata = new LinkedHashMap<String,PDSValue>();
    for (Map.Entry<String,Object> datum : metadata.entrySet()) {
      this.metadata.put(datum.getKey(), new PDSValue(datum.getValue()));
    }

    this.objects = new LinkedHashMap<String,PDSObject>();
    for (Map.Entry<String,Map<String,Object>> oe : objects.entrySet()) {
      String name = oe.getKey();
      this.objects.put(name, new PDSObject(name, oe.getValue()));
    }
  }

  public String getVersion() {
    return version;
  }
}
