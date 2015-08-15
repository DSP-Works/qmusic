/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "GeneratorSine.h"

#define RAD(x) ((x) * M_PI / 180.0);

const QColor cDefaultColor(180, 250, 220);

GeneratorSine::GeneratorSine(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_phase(0.0)
{
    m_pInputFreq = addInput("f", Signal::Type_Float);
    m_pOutput = addOutput("out", Signal::Type_Float);

    createProperties();
}

QColor GeneratorSine::color() const
{
    return cDefaultColor;
}

void GeneratorSine::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["frequencyScale"] = m_pPropFreqScale->value();
    data["amplitude"] = m_pPropAmplitude->value();
    data["phase"] = m_pPropPhase->value();
    AudioUnit::serialize(data, pContext);
}

void GeneratorSine::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropFreqScale->setValue(data["frequencyScale"]);
    m_pPropAmplitude->setValue(data["amplitude"]);
    m_pPropPhase->setValue(data["phase"]);
    AudioUnit::deserialize(data, pContext);
}

void GeneratorSine::processStart()
{
    m_phase = m_pPropPhase->value().toDouble();
}

void GeneratorSine::processStop()
{
}

void GeneratorSine::process()
{
    float amp = m_pPropAmplitude->value().toDouble();
    float out = amp * sin(m_phase * 2.0 * M_PI);

    float freqScale = m_pPropFreqScale->value().toDouble();
    ISignalChain* chain = signalChain();
    float dt = chain->timeStep();
    float f = m_pInputFreq->value().asFloat;

    float dPhase = f * freqScale * dt;
    m_phase = fmod(m_phase + dPhase, 1.0);
\
    m_pOutput->setFloatValue(out);
}

void GeneratorSine::reset()
{
    m_phase = RAD(m_pPropPhase->value().toDouble());
}

void GeneratorSine::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropFreqScale = propertyManager()->addProperty(QVariant::Double, "Frequency scale");
    m_pPropFreqScale->setAttribute("minimum", 0.01);
    m_pPropFreqScale->setAttribute("maximum", 100.0);
    m_pPropFreqScale->setAttribute("singleStep", 1.0);
    m_pPropFreqScale->setValue(1.0);

    m_pPropAmplitude = propertyManager()->addProperty(QVariant::Double, "Amplitude");
    m_pPropAmplitude->setAttribute("minimum", 0.0);
    m_pPropAmplitude->setAttribute("maximum", 1.0);
    m_pPropAmplitude->setValue(1.0);

    m_pPropPhase = propertyManager()->addProperty(QVariant::Double, "Phase, \302\260");
    m_pPropPhase->setAttribute("minumum", -180.0);
    m_pPropPhase->setAttribute("maximum", 180);
    m_pPropPhase->setValue(0.0);

    pRoot->addSubProperty(m_pPropFreqScale);
    pRoot->addSubProperty(m_pPropAmplitude);
    pRoot->addSubProperty(m_pPropPhase);
}
