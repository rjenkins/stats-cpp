//
// Created by rjenkins on 6/5/20.
//
#include "engine.h"
#include "print_handler.h"
#include "test_handler.h"
#include "gtest/gtest.h"

TEST(Engine, testConstructNoHandler) {
    Engine e("foo");
    ASSERT_EQ("foo", e.prefix);
    ASSERT_EQ(0, e.GetHandlers().size());
    Engine e1(std::string("bar"));
    ASSERT_EQ("bar", e1.prefix);
    ASSERT_EQ(0, e1.GetHandlers().size());
}
TEST(Engine, testConstructWithHandler) {
    Engine e("foo", std::move(std::make_unique<TestHandler>()));
    ASSERT_EQ("foo", e.prefix);
    ASSERT_EQ(1, e.GetHandlers().size());
    ASSERT_NE(nullptr, dynamic_cast<TestHandler *>(e.GetHandlers()[0].get()));
    Engine e1(std::string("foo"), std::move(std::make_unique<TestHandler>()));
    ASSERT_EQ("foo", e1.prefix);
    ASSERT_EQ(1, e1.GetHandlers().size());
    ASSERT_NE(nullptr, dynamic_cast<TestHandler *>(e1.GetHandlers()[0].get()));
}

TEST(Engine, testConstructWithHandlerVector) {
    std::vector<std::unique_ptr<Handler>> handlers;
    handlers.push_back(std::make_unique<PrintHandler>());
    handlers.push_back(std::make_unique<TestHandler>());
    Engine e("foo", handlers);
    ASSERT_EQ("foo", e.prefix);
    ASSERT_EQ(2, e.GetHandlers().size());
    ASSERT_NE(nullptr, dynamic_cast<PrintHandler *>(e.GetHandlers()[0].get()));
    ASSERT_NE(nullptr, dynamic_cast<TestHandler *>(e.GetHandlers()[1].get()));
}
TEST(Engine, testConstructAddHandler) {
    Engine e("foo");
    ASSERT_EQ("foo", e.prefix);
    ASSERT_EQ(0, e.GetHandlers().size());
    e.RegisterHandler(std::move(std::make_unique<PrintHandler>()));
    ASSERT_EQ(1, e.GetHandlers().size());
    ASSERT_NE(nullptr, dynamic_cast<PrintHandler *>(e.GetHandlers()[0].get()));
}
TEST(Engine, testIncrNoTags) {
    Engine e("test", std::make_unique<TestHandler>());
    e.Incr("foo");
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.foo", im.name);
    ASSERT_EQ(MetricType::Counter, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(1, im.value);
    ASSERT_EQ(0, im.tags.size());
}
TEST(Engine, testIncrWithTags) {
    Engine e("test", std::make_unique<TestHandler>());
    e.Incr("foo", Tag("Hello", "World"), Tag("Another", "Tag"));
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.foo", im.name);
    ASSERT_EQ(MetricType::Counter, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(1, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Hello", im.tags[0].name);
    ASSERT_EQ("World", im.tags[0].value);
    ASSERT_EQ("Another", im.tags[1].name);
    ASSERT_EQ("Tag", im.tags[1].value);
}
TEST(Engine, testIncrTagsVector) {
    Engine e("test", std::make_unique<TestHandler>());
    std::vector<Tag> tags;
    tags.reserve(2);
    Tag::appendTag(tags, "Foo", "Bar");
    Tag::appendTag(tags, "Hello", "World");

    e.Incr("foo", tags);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.foo", im.name);
    ASSERT_EQ(MetricType::Counter, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(1, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Foo", im.tags[0].name);
    ASSERT_EQ("Bar", im.tags[0].value);
    ASSERT_EQ("Hello", im.tags[1].name);
    ASSERT_EQ("World", im.tags[1].value);
}
TEST(Engine, testAddIntegerNoTags) {
    Engine e("test", std::make_unique<TestHandler>());
    e.Add("foo", 100000000);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.foo", im.name);
    ASSERT_EQ(MetricType::Counter, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100000000, im.value);
    ASSERT_EQ(0, im.tags.size());
}
TEST(Engine, testAddIntegerWithTags) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    e.Add("foo", 100000000, Tag("Hello", "World"), Tag("Another", "Tag"));
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.foo", im.name);
    ASSERT_EQ(MetricType::Counter, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100000000, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Hello", im.tags[0].name);
    ASSERT_EQ("World", im.tags[0].value);
    ASSERT_EQ("Another", im.tags[1].name);
    ASSERT_EQ("Tag", im.tags[1].value);
}
TEST(Engine, testAddIntegerWithTagsVector) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    std::vector<Tag> tags;
    tags.reserve(2);
    Tag::appendTag(tags, "Hello", "World");
    Tag::appendTag(tags, "Another", "Tag");
    e.Add("foo", 100000000, tags);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.foo", im.name);
    ASSERT_EQ(MetricType::Counter, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100000000, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Hello", im.tags[0].name);
    ASSERT_EQ("World", im.tags[0].value);
    ASSERT_EQ("Another", im.tags[1].name);
    ASSERT_EQ("Tag", im.tags[1].value);
}
TEST(Engine, testAddDoubleNoTags) {
    Engine e("test", std::make_unique<TestHandler>());
    e.Add("foo", 10.5);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto im = th->doubleMeasures[0];
    ASSERT_EQ("test.foo", im.name);
    ASSERT_EQ(MetricType::Counter, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(10.5, im.value);
    ASSERT_EQ(0, im.tags.size());
}
TEST(Engine, testAddDoubleWithTags) {
    Engine e("test", std::make_unique<TestHandler>());
    e.Add("foo", 10.5, Tag("Hello", "World"), Tag("Another", "Tag"));
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto dm = th->doubleMeasures[0];
    ASSERT_EQ("test.foo", dm.name);
    ASSERT_EQ(MetricType::Counter, dm.type);
    ASSERT_EQ(1, dm.rate);
    ASSERT_EQ(10.5, dm.value);
    ASSERT_EQ(2, dm.tags.size());
    ASSERT_EQ("Hello", dm.tags[0].name);
    ASSERT_EQ("World", dm.tags[0].value);
    ASSERT_EQ("Another", dm.tags[1].name);
    ASSERT_EQ("Tag", dm.tags[1].value);
}
TEST(Engine, testAddDoubleWithTagsVector) {
    Engine e("test", std::make_unique<TestHandler>());
    std::vector<Tag> tags;
    tags.reserve(2);
    Tag::appendTag(tags, "Hello", "World");
    Tag::appendTag(tags, "Another", "Tag");
    e.Add("foo", 10.5, tags);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto dm = th->doubleMeasures[0];
    ASSERT_EQ("test.foo", dm.name);
    ASSERT_EQ(MetricType::Counter, dm.type);
    ASSERT_EQ(1, dm.rate);
    ASSERT_EQ(10.5, dm.value);
    ASSERT_EQ(2, dm.tags.size());
    ASSERT_EQ("Hello", dm.tags[0].name);
    ASSERT_EQ("World", dm.tags[0].value);
    ASSERT_EQ("Another", dm.tags[1].name);
    ASSERT_EQ("Tag", dm.tags[1].value);
}
TEST(Engine, testSetIntegerNoTags) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    e.Set("gauge.value", 100);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.gauge.value", im.name);
    ASSERT_EQ(MetricType::Gauge, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100, im.value);
    ASSERT_EQ(0, im.tags.size());
}

TEST(Engine, testSetIntegerWithTags) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    e.Set("gauge.value", 100, Tag("An", "Integer"), Tag("Another", "Tag"));
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.gauge.value", im.name);
    ASSERT_EQ(MetricType::Gauge, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("An", im.tags[0].name);
    ASSERT_EQ("Integer", im.tags[0].value);
    ASSERT_EQ("Another", im.tags[1].name);
    ASSERT_EQ("Tag", im.tags[1].value);
}

TEST(Engine, testSetIntegerWithTagsVector) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    std::vector<Tag> tags;
    tags.reserve(2);
    Tag::appendTag(tags, "Foo", "Bar");
    Tag::appendTag(tags, "Hello", "World");
    e.Set("gauge.value", 100, tags);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.gauge.value", im.name);
    ASSERT_EQ(MetricType::Gauge, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Foo", im.tags[0].name);
    ASSERT_EQ("Bar", im.tags[0].value);
    ASSERT_EQ("Hello", im.tags[1].name);
    ASSERT_EQ("World", im.tags[1].value);
}
TEST(Engine, testSetDoubleNoTags) {
    Engine e("test", std::make_unique<TestHandler>());
    e.Set("gauge.value", 100.0005);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto im = th->doubleMeasures[0];
    ASSERT_EQ("test.gauge.value", im.name);
    ASSERT_EQ(MetricType::Gauge, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100.0005, im.value);
    ASSERT_EQ(0, im.tags.size());
}

TEST(Engine, testSetDoubleWithTags) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    e.Set("gauge.value", 100.0005, Tag("Foo", "Bar"), Tag("Hello", "World"));
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto im = th->doubleMeasures[0];
    ASSERT_EQ("test.gauge.value", im.name);
    ASSERT_EQ(MetricType::Gauge, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100.0005, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Foo", im.tags[0].name);
    ASSERT_EQ("Bar", im.tags[0].value);
    ASSERT_EQ("Hello", im.tags[1].name);
    ASSERT_EQ("World", im.tags[1].value);
}
TEST(Engine, testSetDoubleTagsVector) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    std::vector<Tag> tags;
    tags.reserve(2);
    Tag::appendTag(tags, "Foo", "Bar");
    Tag::appendTag(tags, "Hello", "World");
    e.Set("gauge.value", 100.0005, tags);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto im = th->doubleMeasures[0];
    ASSERT_EQ("test.gauge.value", im.name);
    ASSERT_EQ(MetricType::Gauge, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100.0005, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Foo", im.tags[0].name);
    ASSERT_EQ("Bar", im.tags[0].value);
    ASSERT_EQ("Hello", im.tags[1].name);
    ASSERT_EQ("World", im.tags[1].value);
}
TEST(Engine, testObserveIntegerNoTags) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    e.Observe("histo.value", 42);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.histo.value", im.name);
    ASSERT_EQ(MetricType::Histogram, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(42, im.value);
    ASSERT_EQ(0, im.tags.size());
}
TEST(Engine, testObserveIntegerWithTags) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    e.Observe("histo.value", 3, Tag("foo", "bar"), Tag("another", "tag"));
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.histo.value", im.name);
    ASSERT_EQ(MetricType::Histogram, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(3, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("foo", im.tags[0].name);
    ASSERT_EQ("bar", im.tags[0].value);
    ASSERT_EQ("another", im.tags[1].name);
    ASSERT_EQ("tag", im.tags[1].value);
}
TEST(Engine, testObserveIntegerWithTagsVector) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    std::vector<Tag> tags;
    tags.reserve(2);
    Tag::appendTag(tags, "Foo", "Bar");
    Tag::appendTag(tags, "Hello", "World");
    e.Observe("histo.value", 3, tags);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->intMeasures.size());
    auto im = th->intMeasures[0];
    ASSERT_EQ("test.histo.value", im.name);
    ASSERT_EQ(MetricType::Histogram, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(3, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Foo", im.tags[0].name);
    ASSERT_EQ("Bar", im.tags[0].value);
    ASSERT_EQ("Hello", im.tags[1].name);
    ASSERT_EQ("World", im.tags[1].value);
}
TEST(Engine, testObserveDoubleNoTags) {
    Engine e("test", std::make_unique<TestHandler>());
    e.Observe("histo.value", 100.0005);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto im = th->doubleMeasures[0];
    ASSERT_EQ("test.histo.value", im.name);
    ASSERT_EQ(MetricType::Histogram, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100.0005, im.value);
    ASSERT_EQ(0, im.tags.size());
}

TEST(Engine, testObserveDoubleWithTags) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    e.Observe("histo.value", 100.0005, Tag("Foo", "Bar"), Tag("Hello", "World"));
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto im = th->doubleMeasures[0];
    ASSERT_EQ("test.histo.value", im.name);
    ASSERT_EQ(MetricType::Histogram, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100.0005, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Foo", im.tags[0].name);
    ASSERT_EQ("Bar", im.tags[0].value);
    ASSERT_EQ("Hello", im.tags[1].name);
    ASSERT_EQ("World", im.tags[1].value);
}
TEST(Engine, testObserveDoubleTagsVector) {
    Engine e("test", std::move(std::make_unique<TestHandler>()));
    std::vector<Tag> tags;
    tags.reserve(2);
    Tag::appendTag(tags, "Foo", "Bar");
    Tag::appendTag(tags, "Hello", "World");
    e.Observe("histo.value", 100.0005, tags);
    const auto &h = e.GetHandlers()[0];
    auto *th = dynamic_cast<TestHandler *>(h.get());
    ASSERT_EQ(1, th->doubleMeasures.size());
    auto im = th->doubleMeasures[0];
    ASSERT_EQ("test.histo.value", im.name);
    ASSERT_EQ(MetricType::Histogram, im.type);
    ASSERT_EQ(1, im.rate);
    ASSERT_EQ(100.0005, im.value);
    ASSERT_EQ(2, im.tags.size());
    ASSERT_EQ("Foo", im.tags[0].name);
    ASSERT_EQ("Bar", im.tags[0].value);
    ASSERT_EQ("Hello", im.tags[1].name);
    ASSERT_EQ("World", im.tags[1].value);
}

//TEST(Engine, testPrintHandler) {
//    Engine e("printHandlerTest", std::move(std::make_unique<PrintHandler>()));
//    e.Add("foo", 5.5, Tag("Hello", "World"));
//}
//
//TEST(Engine, testDDHandler) {
//    Engine e("test", std::move(std::make_unique<DatadogHandler>()));
//    e.Incr("foo", Tag("foo", "bar"));
//}
