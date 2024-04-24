require("dotenv").config();

module.exports = (sequelize, DataTypes) => {
    return sequelize.define("web_store_products",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            name: {
                type: DataTypes.STRING(64),
                allowNull: false
            },
            description: {
                type: DataTypes.STRING(256),
                allowNull: false
            },
            type: {
                type: DataTypes.STRING(64),
                allowNull: false
            },
            value: {
                type: DataTypes.STRING(128),
                allowNull: true
            },
            price: {
                type: DataTypes.FLOAT,
                allowNull: false
            },
            currency: {
                type: DataTypes.STRING(10),
                allowNull: false,
                defaultValue: process.env.DEFAULT_WEBSTORE_CURRENCY
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};