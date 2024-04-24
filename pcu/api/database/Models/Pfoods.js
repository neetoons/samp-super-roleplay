module.exports = (sequelize, DataTypes) => {
    return sequelize.define("pfoods",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            name: {
                type: DataTypes.STRING(24),
                allowNull: false
            },
            hungry: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            thirst: {
                type: DataTypes.FLOAT,
                allowNull: false,
                defaultValue: 0.0
            },
            drunk: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};