#include "characterdsa5viewer.h"

#include <QDebug>
#include <QFile>
#include <QtMath>

CharacterDSA5Viewer::CharacterDSA5Viewer(QQmlApplicationEngine *engine)
    : qmlEngine(engine)
{
    qDebug() << "Loading CharacterDSA5Viewer ...";

    talentsModel   = new DSA5ListModel;
    combatModel    = new DSA5ListModel;
    specialModel   = new DSA5ListModel;
    liturgiesModel = new DSA5ListModel;
    spellsModel    = new DSA5ListModel;
    itemsModel     = new DSA5ListModel;

    qmlEngine->rootContext()->setContextProperty("dsa5TalentsModel", talentsModel);
    qmlEngine->rootContext()->setContextProperty("dsa5CombatModel", combatModel);
    qmlEngine->rootContext()->setContextProperty("dsa5SpecialModel", specialModel);
    qmlEngine->rootContext()->setContextProperty("dsa5LiturgiesModel", liturgiesModel);
    qmlEngine->rootContext()->setContextProperty("dsa5SpellsModel", spellsModel);
    qmlEngine->rootContext()->setContextProperty("dsa5ItemsModel", itemsModel);

    m_categories.append({ tr("Profil"), tr("Eigenschaften"), tr("Vorteile & Nachteile"), tr("Fertigkeiten"), tr("Besitz") });

    for (int i = 0; i < 20; i++) m_profileOverview.append("");

    for (int i = 0; i < 17; i++) m_attributes.append("");

    loadReferenceObject();
}

void CharacterDSA5Viewer::setCharacter(Character *character)
{
    m_currentCharacter = character;
    loadCharacterData();

    emit categoriesChanged();
    emit characterChanged();
}

void CharacterDSA5Viewer::setCurrentCategory(int index)
{
    m_categoryIndex = index;
    emit categoryChanged();
    setCurrentSubCategory(0);
}

QStringList CharacterDSA5Viewer::subCategories() const
{
    switch (m_categoryIndex)
    {
    case 0: return { tr("Übersicht"), tr("Regeln") };

    case 3: return { tr("Talente"), tr("Kampftechniken"), tr("Sonderfertigkeiten"), tr("Zauber"), tr("Liturgien") };

    case 4: return { tr("Ausrüstung"), tr("Begleiter") };

    default: return {};
    }
}

void CharacterDSA5Viewer::loadCharacterData()
{
    m_image.clear();
    m_profileOverview.clear();

    if (!m_currentCharacter || (m_currentCharacter->files().size() == 0)) return;

    QFile f(m_currentCharacter->files()[0].path());

    if (f.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        f.close();

        m_jsonObject = doc.object();

        loadProfile();
        loadRules();
        loadAttributes();
        loadAdvantages();
        loadDisadvantages();
        loadTalents();
        loadCombat();
        loadSpecial();
        loadItems();
    }
}

void CharacterDSA5Viewer::loadProfile()
{
    m_image = jsonString("avatar");

    // Name
    m_profileOverview.append(jsonString("name"));

    // Sex
    m_profileOverview.append(jsonString("sex") == "m" ? "Männlich" : "Weiblich");

    // Race + (Race variant)
    QString rTemp  = m_jsonObject.value("r").toString();
    int     rIndex = rTemp.right(rTemp.length() - rTemp.lastIndexOf('_') - 1).toInt() - 1;
    m_race = m_referenceObject.value("R").toArray()[rIndex].toObject();
    m_profileOverview.append(m_race.value("name").toString() + " (" + jsonMapString(jsonString("rv")) + ")");

    // Culture
    m_profileOverview.append(jsonMapString(jsonString("c")));

    // Profession
    m_profileOverview.append(jsonString("professionName"));

    // Experience level
    m_profileOverview.append(jsonMapString(jsonString("el")));

    // Experience value
    m_profileOverview.append(QString::number(m_jsonObject.value("ap").toObject().value("total").toInt()) + " AP");

    // Personal Data
    auto pers = m_jsonObject.value("pers").toObject();

    // Family
    m_profileOverview.append(pers.value("family").toString());

    // Place of birth
    m_profileOverview.append(pers.value("placeofbirth").toString());

    // Date of birth
    m_profileOverview.append(pers.value("dateofbirth").toString());

    // Age
    m_profileOverview.append(pers.value("age").toString());

    // Hair color
    m_profileOverview.append(jsonMapString("haircolor_" + QString::number(pers.value("haircolor").toInt())));

    // Eye color
    m_profileOverview.append(jsonMapString("eyecolor_" + QString::number(pers.value("eyecolor").toInt())));

    // Size
    m_profileOverview.append(pers.value("size").toString());

    // Weight
    m_profileOverview.append(pers.value("weight").toString());

    // Title
    m_profileOverview.append(pers.value("title").toString());

    // Social status
    m_profileOverview.append(m_referenceObject.value("socialstatus").toArray()[pers.value("socialstatus").toInt() - 1].toString());

    // Characteristics
    m_profileOverview.append(pers.value("characteristics").toString());

    // Other info
    m_profileOverview.append(pers.value("otherinfo").toString());

    // Culture area knowledge
    m_profileOverview.append(pers.value("cultureAreaKnowledge").toString());
}

void CharacterDSA5Viewer::loadRules()
{
    m_ruleBooks.clear();
    m_optionalRules.clear();

    auto rules    = m_jsonObject.value("rules").toObject();
    auto allBooks = rules.value("enableAllRuleBooks").toBool();

    // Books
    if (allBooks)
    {
        m_ruleBooks.append(tr("Alle Regelwerke"));
    }
    else
    {
        auto books = rules.value("enabledRuleBooks").toArray();

        for (auto b : books) m_ruleBooks.append(m_referenceObject.value("rulebooks").toObject().value(b.toString()).toString());
    }

    // Optional rules
    auto paradeValues = rules.value("higherParadeValues").toInt();

    if (paradeValues > 0) m_optionalRules.append(tr("Höhere Verteidigungswerte: +") + QString::number(paradeValues));

    if (rules.value("attributeValueLimit").toBool()) m_optionalRules.append(tr("Eigenschaftsobergrenze"));

    if (rules.value("enableLanguageSpecializations").toBool()) m_optionalRules.append(tr("Sprachspezialisierungen"));
}

void CharacterDSA5Viewer::loadAttributes()
{
    m_attributes.clear();

    auto attr   = m_jsonObject.value("attr").toObject();
    auto values = attr.value("values").toArray();

    for (auto v : values)
    {
        if (v.toArray().size() > 1) m_attributes.append(QString::number(v.toArray()[1].toInt()));
        else m_attributes.append("0");
    }

    // Life
    m_attributes.append(QString::number(m_attributes[6].toInt() * 2 + m_race.value("lp").toInt()));

    // AE TODO
    m_attributes.append("-");

    // KE TODO
    m_attributes.append("-");

    // SK
    m_attributes.append(QString::number(m_race.value("sk").toInt() + (static_cast<qreal>(m_attributes[0].toInt()) + m_attributes[1].toInt() + m_attributes[2].toInt()) / 6, 'f', 0));

    // ZK
    m_attributes.append(QString::number(m_race.value("sk").toInt() + (static_cast<qreal>(m_attributes[6].toInt() * 2) + m_attributes[7].toInt()) / 6, 'f', 0));

    // AW
    m_attributes.append(QString::number(m_attributes[5].toInt() * 0.5, 'f', 0));

    // INI
    m_attributes.append(QString::number((m_attributes[0].toInt() + m_attributes[5].toInt()) * 0.5, 'f', 0));

    // GS
    m_attributes.append(QString::number(m_race.value("gs").toInt()));

    // WS
    m_attributes.append(QString::number(m_attributes[6].toInt() * 0.5, 'f', 0));
}

void CharacterDSA5Viewer::loadTalents()
{
    talentsModel->clear();

    auto refTalents  = m_referenceObject.value("talents").toArray();
    auto charTalents = m_jsonObject.value("talents").toObject();

    QList<int> talentValues;
    QList<DSA5ListItem *> items;

    for (int i = 0; i < 59; i++) talentValues.append(0);

    for (auto t : charTalents.keys())
    {
        int index = t.right(t.length() - t.lastIndexOf("_") - 1).toInt() - 1;
        int value = charTalents.value(t).toInt();
        talentValues.replace(index, value);
    }

    for (int i = 0; i < refTalents.size(); i++)
    {
        auto t            = refTalents[i].toObject();
        QList<int> values = { talentValues[i] };

        for (auto v : t.value("attributes").toArray()) values.append(m_attributes[v.toInt()].toInt());

        items.append(new DSA5ListItem(t.value("name").toString(), t.value("group").toString(), values));
    }

    talentsModel->setElements(items);
}

void CharacterDSA5Viewer::loadAdvantages()
{
    m_advantages.clear();

    auto refAdvantages   = m_referenceObject.value("advantages").toArray();
    auto charActivatable = m_jsonObject.value("activatable").toObject();
    QList<int> indices;

    for (auto a : charActivatable.keys())
    {
        if (a.startsWith("ADV_")) indices.append(a.right(a.length() - a.lastIndexOf('_') - 1).toInt() - 1);
    }

    for (int i : indices)
    {
        m_advantages.append(refAdvantages[i].toObject().value("name").toString());
    }
}

void CharacterDSA5Viewer::loadDisadvantages()
{
    m_disadvantages.clear();

    auto refDisadvantages = m_referenceObject.value("disadvantages").toArray();
    auto charActivatable  = m_jsonObject.value("activatable").toObject();
    QList<int> indices;

    for (auto a : charActivatable.keys())
    {
        if (a.startsWith("DISADV_")) indices.append(a.right(a.length() - a.lastIndexOf('_') - 1).toInt() - 1);
    }

    for (int i : indices)
    {
        m_disadvantages.append(refDisadvantages[i].toObject().value("name").toString());
    }
}

void CharacterDSA5Viewer::loadCombat()
{
    combatModel->clear();

    auto refCombat  = m_referenceObject.value("combat").toArray();
    auto charCombat = m_jsonObject.value("ct").toObject();

    QList<int> talentValues;
    QList<DSA5ListItem *> items;

    for (int i = 0; i < 17; i++) talentValues.append(6);

    for (auto t : charCombat.keys())
    {
        int index = t.right(t.length() - t.lastIndexOf("_") - 1).toInt() - 1;
        int value = charCombat.value(t).toInt();
        talentValues.replace(index, value);
    }

    for (int i = 0; i < refCombat.size(); i++)
    {
        auto t            = refCombat[i].toObject();
        QList<int> values = { talentValues[i] };

        values.append(values[0] + qFloor(m_attributes[0].toInt() - 8));

        if (t.value("parade").toBool())
        {
            bool fk = t.value("group").toString() == "Fernkampf";
            values.append(qRound(static_cast<qreal>(values[fk ? 4 : 0]) / 2) + qFloor(m_attributes[t.value("attribute").toInt()].toInt() - 8));
        }
        else
        {
            values.append(0);
        }

        items.append(new DSA5ListItem(t.value("name").toString(), t.value("group").toString(), values));
    }

    combatModel->setElements(items);
}

void CharacterDSA5Viewer::loadSpecial()
{
    m_special.clear();

    auto refSpecial      = m_referenceObject.value("special_abilities").toArray();
    auto charActivatable = m_jsonObject.value("activatable").toObject();
    QList<int> indices;

    for (auto a : charActivatable.keys())
    {
        if (a.startsWith("SA_")) indices.append(a.right(a.length() - a.lastIndexOf('_') - 1).toInt() - 1);
    }

    for (int i : indices)
    {
        m_special.append(refSpecial[i].toObject().value("name").toString());
    }
}

void CharacterDSA5Viewer::loadItems()
{
    itemsModel->clear();
    auto itemObject = m_jsonObject.value("belongings").toObject().value("items").toObject();
    QList<DSA5ListItem *> items;

    for (auto i : itemObject.keys())
    {
        QString name   = itemObject.value(i).toObject().value("name").toString();
        QString note   = itemObject.value(i).toObject().value("note").toString();
        QString where  = itemObject.value(i).toObject().value("where").toString();
        int     amount = itemObject.value(i).toObject().value("amount").toInt();

        if (!note.isEmpty()) name.append(" (" + note + ")");

        items.append(new DSA5ListItem(name, where, { amount }));
    }

    itemsModel->setElements(items);
}

void CharacterDSA5Viewer::loadReferenceObject()
{
    QFile f(":/addons/dsa5/character_tool.json");

    if (f.open(QIODevice::ReadOnly))
    {
        m_referenceObject = QJsonDocument::fromJson(f.readAll()).object();
        f.close();
    }
}

QString CharacterDSA5Viewer::jsonString(QString key)
{
    return m_jsonObject.value(key).toString();
}

QString CharacterDSA5Viewer::jsonMapString(QString key, bool shiftIndex)
{
    if (key.isEmpty()) return "";

    QString arrayName = key.left(key.lastIndexOf("_"));
    int     index     = key.right(key.length() - key.lastIndexOf("_") - 1).toInt();

    if (shiftIndex) index -= 1;

    auto array = m_referenceObject.value(arrayName).toArray();

    if ((index > -1) && (index < array.size())) return array[index].toString();

    return "";
}
