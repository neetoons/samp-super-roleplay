module.exports = (sequelize, DataTypes) => {
    return sequelize.define("works",
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
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};