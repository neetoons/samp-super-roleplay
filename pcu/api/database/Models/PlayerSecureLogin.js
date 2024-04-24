module.exports = (sequelize, DataTypes) => {
    return sequelize.define("player_secure_login",
        {
            ip: {
                type: DataTypes.STRING(16),
                allowNull: false,
                primaryKey: true
            },
            last_connection: {
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