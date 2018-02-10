/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Common.h"
#include "Delta.h"

using namespace VCS;

static const String undefinedDelta = "undefined";

int64 DeltaDescription::defaultNumChanges = -1;

Delta::Delta(const Delta &other) :
    type(other.type),
    description(other.description),
    vcsUuid(other.vcsUuid)
{
}

String Delta::getHumanReadableText() const
{
    return this->description.getFullText();
}

DeltaDescription Delta::getDescription() const
{
    return this->description;
}

void Delta::setDescription(const DeltaDescription &newDescription)
{
    this->description = newDescription;
}

Uuid Delta::getUuid() const
{
    return this->vcsUuid;
}

String Delta::getType() const
{
    return this->type;
}

ValueTree VCS::Delta::serialize() const
{
    auto const xml = new XmlElement(Serialization::VCS::delta);

    tree.setProperty(Serialization::VCS::deltaType, this->type);
    tree.setProperty(Serialization::VCS::deltaName, this->description.stringToTranslate);
    tree.setProperty(Serialization::VCS::deltaStringParam, this->description.stringParameter);
    tree.setProperty(Serialization::VCS::deltaIntParam, String(this->description.intParameter));
    tree.setProperty(Serialization::VCS::deltaId, this->vcsUuid.toString());

    return tree;
}

void VCS::Delta::deserialize(const ValueTree &tree)
{
    this->reset();

    const XmlElement *root = tree.hasTagName(Serialization::VCS::delta) ?
                             &tree : tree.getChildByName(Serialization::VCS::delta);

    if (root == nullptr) { return; }

    this->vcsUuid = root->getStringAttribute(Serialization::VCS::deltaId, this->vcsUuid.toString());
    this->type = root->getStringAttribute(Serialization::VCS::deltaType, undefinedDelta);

    const String descriptionName = root->getStringAttribute(Serialization::VCS::deltaName, String::empty);
    const String descriptionStringParam = root->getStringAttribute(Serialization::VCS::deltaStringParam, String::empty);
    const int64 descriptionIntParam =
        root->getStringAttribute(Serialization::VCS::deltaIntParam,
                                 String(DeltaDescription::defaultNumChanges)).getLargeIntValue();
    
    this->description = DeltaDescription(descriptionName, descriptionIntParam, descriptionStringParam);
}

void Delta::reset()
{

}
