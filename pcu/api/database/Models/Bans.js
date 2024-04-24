module.exports = (sequelize, DataTypes) => {
    return sequelize.define("bans",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            name: {
                type: DataTypes.STRING(24),
                allowNull: true
            },
            ip: {
                type: DataTypes.STRING(16),
                allowNull: false
            },
            expire_date: {
                type: DataTypes.DATE,
                allowNull: false,
                defaultValue: new Date(0, 0, 0, 0, 0, 0, 0)
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};