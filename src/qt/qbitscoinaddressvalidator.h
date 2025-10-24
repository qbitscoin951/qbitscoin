// Copyright (c) 2011-2020 The Qbitscoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef QBITSCOIN_QT_QBITSCOINADDRESSVALIDATOR_H
#define QBITSCOIN_QT_QBITSCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class QbitscoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit QbitscoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Qbitscoin address widget validator, checks for a valid qbitscoin address.
 */
class QbitscoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit QbitscoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // QBITSCOIN_QT_QBITSCOINADDRESSVALIDATOR_H
