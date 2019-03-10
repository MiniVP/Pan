# Pan Library

This library provides a base class with common code for all Pan weather stations.  
Consumers should write a class that extends the `Pan` class and provide two methods:

* `void begin()`: Initialize components.
* `String buildOutput()`: Return a comma-separated list of sensor values.

## Example

``` cpp
class PanDemo: public Pan {
  public:
    PanDemo(): Pan() {
    }
    void begin() {
      // Initialize the base class
      Pan::begin();
      // Initialize anything else
      someSensor.begin();
    }
    String buildOutput() {
      // Get base output from base class (for example, the timestamp)
      String out = Pan::buildOutput();
      // Add sensor outputs
      out.concat(someSensor.getOutput1());
      // Separate other outputs with commas
      out.concat(F(","));
      out.concat(someSensor.getOutput2());
      return out;
    }
}

// Arduino code

PanDemo panDemo;

void setup() {
  panDemo.begin();
}

void loop() {
  panDemo.loop();
}
```
