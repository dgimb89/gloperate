#include <gloperate/pipelines/AbstractPipeline.h>

#include <cassert>

#include <string>
#include <gloperate/util/collection.hpp>

#include <algorithm>
#include <map>
#include <iostream>

#include <gloperate/pipelines/AbstractStage.h>
#include <gloperate/pipelines/AbstractInputSlot.h>
#include <gloperate/pipelines/AbstractData.h>
#include <gloperate/pipelines/AbstractData.h>
#include <gloperate/pipelines/Data.h>

using namespace collection;

namespace gloperate
{

AbstractPipeline::AbstractPipeline(const std::string & name)
: m_initialized(false)
, m_name(name)
, m_dependenciesSorted(false)
{
}

AbstractPipeline::~AbstractPipeline()
{
    for (AbstractStage* stage : m_stages)
    {
        delete stage;
    }

    for (AbstractData* data : m_constantParameters)
    {
        delete data;
    }
}

const std::string & AbstractPipeline::name() const
{
    return m_name;
}

void AbstractPipeline::setName(const std::string & name)
{
    m_name = name;
}

bool AbstractPipeline::hasName() const
{
    return !m_name.empty();
}

std::string AbstractPipeline::asPrintable() const
{
    if (!hasName())
        return "<unnamed>";

    std::string n = name();
    std::replace(n.begin(), n.end(), ' ', '_');

    return n;
}

void AbstractPipeline::addStages()
{
}

void AbstractPipeline::addStage(AbstractStage* stage)
{
    m_stages.push_back(stage);

    stage->dependenciesChanged.connect([this]() { m_dependenciesSorted = false; });
}

void AbstractPipeline::addParameter(AbstractData * parameter)
{
    m_parameters.push_back(parameter);
}

void AbstractPipeline::shareData(const AbstractData* data)
{
    assert(data != nullptr);

    m_sharedData.push_back(data);
}

void AbstractPipeline::shareDataFrom(const AbstractInputSlot& slot)
{
    shareData(slot.connectedData());
}

void AbstractPipeline::addParameter(const std::string & name, AbstractData * parameter)
{
    parameter->setName(name);
    addParameter(parameter);
}

const std::vector<AbstractStage*>& AbstractPipeline::stages() const
{
    return m_stages;
}

const std::vector<AbstractData*> & AbstractPipeline::parameters() const
{
    return m_parameters;
}

std::vector<AbstractInputSlot*> AbstractPipeline::allInputs() const
{
    return flatten(collect(m_stages, [](const AbstractStage * stage) { return stage->allInputs(); }));
}

std::vector<AbstractData*> AbstractPipeline::allOutputs() const
{
    return flatten(collect(m_stages, [](const AbstractStage * stage) { return stage->allOutputs(); }));
}

AbstractData * AbstractPipeline::findParameter(const std::string & name) const
{
    return detect(m_parameters, [&name](AbstractData * parameter) { return parameter->matchesName(name); }, nullptr);
}

std::vector<AbstractData *> AbstractPipeline::findOutputs(const std::string & name) const
{
    return select(allOutputs(), [&name](AbstractData * data) { return data->matchesName(name); });
}

void AbstractPipeline::execute()
{
    if (!m_initialized)
    {
        return;
    }

    if (!m_dependenciesSorted)
    {
        sortDependencies();
    }

    for (AbstractStage* stage: m_stages)
    {
        stage->execute();
    }
}

bool AbstractPipeline::isInitialized() const
{
    return m_initialized;
}

void AbstractPipeline::initialize()
{
    if (m_initialized)
        return;

    initializeStages();

    m_initialized = true;
}

void AbstractPipeline::initializeStages()
{
    if (!m_dependenciesSorted)
    {
        sortDependencies();
    }

    for (AbstractStage * stage : m_stages)
    {
        stage->initialize();
    }
}

void AbstractPipeline::sortDependencies()
{
    if (m_dependenciesSorted)
        return;

    tsort(m_stages);
    m_dependenciesSorted = true;
}

void AbstractPipeline::tsort(std::vector<AbstractStage*>& stages)
{
    std::vector<AbstractStage*> sorted;
    std::map<AbstractStage*, unsigned char> marks;

    std::function<void(AbstractStage*)> visit = [&](AbstractStage * stage)
    {
        if (marks[stage] == 1)
        {
            std::cerr << "Pipeline is not a directed acyclic graph" << std::endl;
            return;
        }

        if (marks[stage] == 0)
        {
            marks[stage] = 1;
            for (auto nextStage : stages)
            {
                if (stage->requires(nextStage, false))
                {
                    visit(nextStage);
                }
            }

            marks[stage] = 2;
            sorted.push_back(stage);
        }
    };

    while (sorted.size() < stages.size())
    {
        for (auto stage : stages)
        {
            if (marks[stage] == 0)
            {
                visit(stage);
            }
        }
    }

    stages.swap(sorted);
}

} // namespace gloperate
