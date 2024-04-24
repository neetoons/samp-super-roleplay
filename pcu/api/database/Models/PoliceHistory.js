module.exports = (sequelize, DataTypes) => {
    return sequelize.define("police_history",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            text: {
                type: DataTypes.STRING(128),
                allowNull: false
            },
            date: {
                type: DataTypes.DATE,
                allowNull: false,
                defaultValue: sequelize.literal("CURRENT_TIMESTAMP")
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};