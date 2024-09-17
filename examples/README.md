# Maplet Example | Mapping Hand Tracking to Eurorack Parameters

Maplet has many different use cases in the Eurorack ecosystem. Here's just one example!

![pinch-demo](https://github.com/user-attachments/assets/97708513-116a-41e8-86bf-fab50740887f)

In this example, we show how to create a mapping between a machine-learning based hand-tracking system to parameters of a Eurorack macro-oscillator using the Maplet module.

## MediaPipe: Hands

[MediaPipe](https://github.com/google-ai-edge/mediapipe) is an open-source project maintained by Google that specializes on integrating machine learning with live-streaming data.

One such solution is MediaPipe: Hands that enables 3-dimensional multi-landmark hand-tracking using commodity web-camera. We've previously integrated a libmapper binding to MediaPipe hands ([avaliable here](https://github.com/peacheym/mp-hands-libmapper)) that exposes various hand-tracking landmarks as libmapper signals.


## Defining Mappings

With both the MediaPipe Hands software as well as the Maplet module up and running, we are able to begin creating mappings between them. Using a libmapper session manager such as [Webmapper](https://github.com/libmapper/webmapper/) or [umapper](https://github.com/libmapper/umapper), we can begin by creating a simple direct mapping between signals.


### Direct Mapping

For instance, creating a mapping between the `index-tip` signal and a channel on Maplet (which can be connected to a CV in for pitch-parameter on another eurorack module) results in a theremin type instrument, where the location of your finger-tip along the x-axis is directly mapped to the pitch of the oscilator.

### Convergent Mapping

Though direct mappings can be powerful tools for creating interactions with Digital Musical Insturments and other multimedia systems, there are other types of mapping topologies that libmapper also supports. For instance, a convergent mapping enables two (or more) outgoing signals to influence the value that is received by an incoming signal across the map. This is demonstrated in the main figure above, where both `index-tip` and `thumb-tip` signals are mapped to a channel of the Maplet module. 

```
y=(x$1-x$0).norm() * 4098;
```

Using a mapping-expression (which defines the exact relationship between data signals) we can measure the distance (in any number of avaliable dimensions) between finger-tips and use that result to emulate a pinch gesture. Thus, we can map the _amount of pinch_ to another CV controlled parameter (such as filter-cutoff for example) of a Eurorack module.

## Other examples.

While these initial examples show the basics of how to use the Maplet module as part of the libmapper workflow, we encourage all uers to try it out with their own systems. Feel free to get in touch with any questions or to show any interesting examples that you've come up with!

## Resources

- libmapper website | [link](https://libmapper.org)
- libmapper repo | [link](https://github.com/libmapper/libmapper)
- libmapper tutorials | [link](http://libmapper.github.io/tutorials/introduction.html)
- libmapper expression engine syntax guide | [https://github.com/libmapper/libmapper/blob/main/doc/expression_syntax.md]
