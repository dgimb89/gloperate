/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <globjects/base/ChangeListener.h>
#include <globjects/base/ref_ptr.h>
#include <globjects/base/AbstractStringSource.h>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class GLOPERATE_API StringSourceDecorator : public globjects::AbstractStringSource, protected globjects::ChangeListener
{


public:
    StringSourceDecorator(globjects::AbstractStringSource * source);
    virtual void update();


protected:
    virtual ~StringSourceDecorator();
    virtual void notifyChanged(const Changeable * changeable) override;


protected:
    globjects::ref_ptr<globjects::AbstractStringSource> m_internal;


};


} // namespace gloperate