module.exports = (sequelize, DataTypes) => {
    return sequelize.define("crews",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            name: {
                type: DataTypes.STRING(32),
                allowNull: false
            },
            color: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: -1
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};