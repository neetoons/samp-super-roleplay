module.exports = (sequelize, DataTypes) => {
    return sequelize.define("web_store_history",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            date: {
                type: DataTypes.DATE,
                allowNull: false,
                defaultValue: sequelize.literal("CURRENT_TIMESTAMP")
            },
            ip: {
                type: DataTypes.STRING(32),
                allowNull: false
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
}